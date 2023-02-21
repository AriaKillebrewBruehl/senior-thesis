#include "caboodle.hpp"

cv::Mat caboodle(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());
    if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
    }

    // step 1: extract the edges of the image
    cv::Mat edges = extractEdges("", image, 300, false);
    if (edges.type() != 0) {
        edges.convertTo(edges, 0);
    }

    std::cout << "extracted edges from image" << std::endl;

    // step 2: extract the isophotes of the image
    cv::Mat isophotes = extractIsophotes("", image, 10, 5, false);
    if (isophotes.type() != 0) {
        isophotes.convertTo(isophotes, 0);
    }
    std::cout << "extracted isophotes from image" << std::endl;

    // step 3: offset map
    cv::Mat map = fullMap("", edges, "", isophotes, 6.0, false);
    std::cout << "extracted offset map from image" << std::endl;

    // step 4: generate final dot placement
    cv::Mat adjusted = dots("", map, false);
    std::cout << "finalized dot placement for image" << std::endl;

    // step 5: place circles!
    cv::Mat rendered = placeDots("", adjusted, "", image, 12, true);
    std::cout << "sized dots" << std::endl;
    save(rendered, path, "-rendered");

    return rendered;
}

int main(int argc, char** argv) {
    cv::CommandLineParser parser(
        argc, argv,
        "{@input   ||input image}"
        "{go G g   |false|process image with no breaks (hit 'G' to process "
        "with no breaks at runtime)}");

    std::cout
        << "This program renders an input photograph as a hedcut drawing\n\n";
    std::cout << "\nPress 'N' / 'n' to move to edge detection step in hedcut "
                 "process.\nPress "
                 "'ESC' to exit program.\nPress 'R' to reset values at "
                 "any step\n";
    parser.printMessage();

    // 1) read in input image
    cv::Mat image;
    bool go = parser.get<bool>("go");
    cv::String image_path = parser.get<cv::String>("@input");

    image = read(image_path, image);
    if (image.empty()) {
        printf("Error opening image: %s\n", image_path.c_str());
        return EXIT_FAILURE;
    }

    for (;;) {
        cv::imshow("Hedcut Demo - Initial Input", image);
        char key = (char)cv::waitKey(0);
        std::cout << key << std::endl;
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'g' || key == 'G' || go) {
            cv::Mat rendered = caboodle(image_path, image, true);
            cv::imshow("Hedcut Demo - Rendered", rendered);
            std::cout << "\nPress any key to exit program.\n";
            char key2 = (char)cv::waitKey(0);
            if (key2) {
                return EXIT_SUCCESS;
            }
        }
        if (key == 'n' || key == 'N') {
            goto EDGE_EXTRACTION;
        }
    }

// 2) loop over edge detection, allow tuning of threshold paramenter
EDGE_EXTRACTION:
    std::cout
        << "Beginning edge detection proccess\nPress 'T' / 't'to increase / "
           "decrease threshold parameter by 25 px for edge detection (initial "
           "value is 300 px).\nPress 'N' / 'n' to move to isophote detection "
           "step.\nPress 'R' to reset values at any step.\nPress 'S'/ 's' to "
           "save current image.\nPress 'ESC' to exit program.";

    cv::Mat edges;
    int thresh = 300;
    edges = extractEdges("", image, thresh, false);
    cv::imshow("Hedcut Demo - Extracted Edges", edges);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'n' || key == 'N') {
            goto ISOPHOTE_DETECTION;
        }
        if (key == 'r' || key == 'R') {
            thresh = 300;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-edges-" + std::to_string(thresh);
            save(edges, image_path, tag);
        }
        if (key == 't') {
            thresh -= 25;
        }
        if (key == 'T') {
            thresh += 25;
        }
        edges = extractEdges("", image, thresh, false);
        cv::imshow("Hedcut Demo - Extracted Edges", edges);
    }

// 3) accept edge image and being isophotes, allow tuning of portion of
ISOPHOTE_DETECTION:
    std::cout
        << "Beginning isophote detection proccess\nPress 'I' / 'i' to increase "
           "/ decrease fraction of isophotes taken to 1/n (initial "
           "value is 1/5).\nPress 'N' / 'n' to move to isophote extraction "
           "step.\nPress 'R' to reset values at any step.\nPress 'S'/ 's' to "
           "save current image.\nPress 'ESC' to exit program.\n ";
    cv::Mat isophotes;
    thresh = 5;
    isophotes = getIsophotes("", image, thresh, true);
    cv::imshow("Hedcut Demo - Detected Isophotes", isophotes);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'n' || key == 'N') {
            goto ISOPHOTE_EXTRACTION;
        }
        if (key == 'r' || key == 'R') {
            thresh = 5;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-detected-isophotes-" + std::to_string(thresh);
            save(isophotes, image_path, tag);
        }
        if (key == 'i') {
            thresh--;
        }
        if (key == 'I') {
            thresh++;
        }
        isophotes = getIsophotes("", image, thresh, false);
        cv::imshow("Hedcut Demo - Detected Isophotes", isophotes);
    }

ISOPHOTE_EXTRACTION:
    std::cout
        << "Beginning isophote extraction proccess\nPress 'T' / 't' to "
           "increase / decrease threshold parameter by 10 px for edge "
           "detection (initial value is 50 px).\nPress 'N' / 'n' to move to"
           " offsetmap step.\nPress 'R' to reset values at any step.\nPress "
           "'S' / 's' to save current image.\nPress 'ESC' to exit program.\n";
    cv::Mat isophotes_extracted;
    thresh = 50;
    cv::Mat map = extractEdges("", isophotes, thresh, false);
    cv::imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'n' || key == 'N') {
            goto OFFSET_MAP;
        }
        if (key == 'r' || key == 'R') {
            thresh = 50;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-isophotes-" + std::to_string(thresh);
            save(isophotes_extracted, image_path, tag);
        }
        if (key == 't') {
            thresh -= 10;
        }
        if (key == 'T') {
            thresh += 10;
        }
        cv::Mat map = extractEdges("", isophotes, thresh, false);
        cv::imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
    }

OFFSET_MAP:
    std::cout << "Beginning offset map proccess\nPress 'L' / 'l' to "
                 "increase / decrease offset lane distance by 0.5 px "
                 "(initial value is 6.0 px).\nPress 'N' / 'n' to move to dot "
                 "adjusting step.\nPress 'R' to reset values at any step.\n"
                 "Press 'S'/ 's' to save current image.\nPress 'ESC' to exit "
                 "program.\n ";

    cv::Mat offset_map;
    float l = 6.0;
    offset_map = fullMap("", edges, "", isophotes_extracted, l, false);
    cv::imshow("Hedcut Demo - Offset Map", offset_map);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'n' || key == 'N') {
            goto ADJUST_DOTS;
        }
        if (key == 'r' || key == 'R') {
            l = 6.0;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-offset-map-" + std::to_string(l);
            save(offset_map, image_path, tag);
        }
        if (key == 'l') {
            thresh -= 0.25;
        }
        if (key == 'L') {
            thresh += 0.25;
        }
        offset_map = fullMap("", edges, "", isophotes_extracted, l, false);
        cv::imshow("Hedcut Demo - Offset Map", offset_map);
    }

ADJUST_DOTS:
    std::cout
        << "Beginning dot adjusting process.\nPress 'N' / 'n' to move to  "
           "circle placement. \nPress 'R' to reset values at any step.\nPress "
           "'S'/ 's' to save current image.\nPress 'ESC' to exit program.\n";
    cv::Mat adjusted_dots;
    adjusted_dots = dots("", offset_map, false);
    cv::imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'n' || 'N') {
            goto PLACE_CIRCLES;
        }
        if (key == 's' || 'S') {
            std::string tag = "-adjusted";
            save(adjusted_dots, image_path, tag);
        }
        // // if (key == 'r' || 'R') {
        // //     l = 6.0;
        // // }
        // // if (key == 'l') {
        // //     thresh -= 0.25;
        // // }
        // // if (key == 'L') {
        // //     thresh += 0.25;
        // }
        adjusted_dots = dots("", offset_map, false);
        cv::imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);
    }

PLACE_CIRCLES:
    std::cout
        << "Beginning circle placement process.\nPress 'D' / 'd' to "
           "increase / decrease maximum circle size by 1 (initial value is "
           "12 px).\nPress 'R' to reset values at any step.\nPress 'S'/ 's' to "
           "save current image.\nPress 'ESC' to exit program.\n";
    cv::Mat rendered;
    int max_size = 12;
    rendered = placeDots("", adjusted_dots, "", image, max_size, false);
    cv::imshow("Hedcut Demo - Rendered", rendered);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 's' || key == 'S') {
            save(rendered, image_path, "-rendered");
        }
        if (key == 'r' || key == 'R') {
            max_size = 12;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-rendered-" + std::to_string(max_size);
            save(rendered, image_path, tag);
        }
        if (key == 'd') {
            thresh--;
        }
        if (key == 'D') {
            thresh++;
        }
        rendered = placeDots("", adjusted_dots, "", image, max_size, false);
        cv::imshow("Hedcut Demo - Rendered", rendered);
    }
    return EXIT_SUCCESS;
}