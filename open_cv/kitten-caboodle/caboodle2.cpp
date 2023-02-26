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
    cv::Mat map = fullMap("", edges, "", isophotes, 6.0, true, false);
    std::cout << "extracted offset map from image" << std::endl;

    // step 4: generate final dot placement
    cv::Mat adjusted = dots("", map, false);
    std::cout << "finalized dot placement for image" << std::endl;

    // step 5: place circles!
    cv::Mat rendered = placeDots("", adjusted, "", image, 20, true);
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
        << "This program renders an input photograph as a hedcut drawing\n\n"
           "At any stage: \n"
           "  press 'B' / 'b' to move to previous step in hedcut "
           "process\n"
           "   press 'N' / 'n' to move to next step in hedcut "
           "process\n"
           "   press 'R' / 'r' to reset parameters to default values\n"
           "   press 'S' / 's' to save current image\n"
           "   press 'ESC' to exit program\n";

    // 1) read in input
    cv::Mat image;
    bool go = parser.get<bool>("go");
    cv::String image_path = parser.get<cv::String>("@input");

    image = read(image_path, image);
    if (image.empty()) {
        printf("Error opening image: %s\n", image_path.c_str());
        return EXIT_FAILURE;
    }

    if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
    }

    // setup variables
    bool auto_save = false;
    cv::Mat edges;
    int thresh_edges = 300;
    cv::Mat isophotes;
    int thresh_iso_highlights = 5;
    cv::Mat isophotes_extracted;
    int thresh_isophotes = 50;
    cv::Mat offset_map;
    cv::Mat offset_map_visual;
    int l = 6.0;
    cv::Mat adjusted_dots;
    cv::Mat rendered;
    int max_size = 15;

SET_UP : {
    std::cout << "Beginning hedcut generation proccess\n"
                 "Press:\n"
                 "  'G' / 'g' to process image with no breaks\n"
                 "  'A' / 'a' to auto save on each step\n";
    for (;;) {
        cv::imshow("Hedcut Demo - Initial Input", image);
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'a' || key == 'A') {
            auto_save = true;
        }
        if (key == 'g' || key == 'G' || go) {
            cv::Mat rendered = caboodle(image_path, image, true);
            cv::imshow("Hedcut Demo - Rendered", rendered);
            std::cout << "\nFinal rendering autosaved.\nPress any key to exit "
                         "program.\n";
            char key2 = (char)cv::waitKey(0);
            if (key2) {
                return EXIT_SUCCESS;
            }
        }
        if (key == 'n' || key == 'N') {
            goto EDGE_EXTRACTION;
        }
    }
}

// 2) loop over edge detection, allow tuning of threshold paramenter
EDGE_EXTRACTION : {
    std::cout << "Beginning edge detection proccess\n\n"
                 "Press:\n"
                 "   'T' / 't'to increase / decrease threshold parameter by 25 "
                 "px for edge detection (initial "
                 "value is 300 px)";

    thresh_edges = 300;
    edges = extractEdges("", image, thresh_edges, false);
    cv::destroyWindow("Hedcut Demo - Initial Input");
    cv::imshow("Hedcut Demo - Extracted Edges", edges);
    for (;;) {
        if (auto_save) {
            std::string tag = "-edges-" + std::to_string(thresh_edges);
            save(edges, image_path, tag);
        }
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Extracted Edges");
            goto SET_UP;
        }
        if (key == 'n' || key == 'N') {
            goto ISOPHOTE_DETECTION;
        }
        if (key == 'r' || key == 'R') {
            thresh_edges = 300;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-edges-" + std::to_string(thresh_edges);
            save(edges, image_path, tag);
        }
        if (key == 't') {
            thresh_edges -= 25;
        }
        if (key == 'T') {
            thresh_edges += 25;
        }
        edges = extractEdges("", image, thresh_edges, false);
        cv::imshow("Hedcut Demo - Extracted Edges", edges);
    }
}
// 3) accept edge image and begin isophotes detection
ISOPHOTE_DETECTION : {
    std::cout << "Beginning isophote detection proccess\n\n"
                 "Press:\n"
                 "   'I' / 'i' to increase / decrease fraction of isophotes "
                 "taken to 1/n (initial value is 1/5)\n ";

    isophotes = getIsophotes("", image, thresh_iso_highlights, true);
    cv::destroyWindow("Hedcut Demo - Extracted Edges");
    cv::imshow("Hedcut Demo - Detected Isophotes", isophotes);
    for (;;) {
        if (auto_save) {
            std::string tag =
                "-detected-isophotes-" + std::to_string(thresh_iso_highlights);
            save(isophotes, image_path, tag);
        }
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Detected Isophotes");
            goto EDGE_EXTRACTION;
        }
        if (key == 'n' || key == 'N') {
            goto ISOPHOTE_EXTRACTION;
        }
        if (key == 'r' || key == 'R') {
            thresh_iso_highlights = 5;
        }
        if (key == 's' || key == 'S') {
            std::string tag =
                "-detected-isophotes-" + std::to_string(thresh_iso_highlights);
            save(isophotes, image_path, tag);
        }
        if (key == 'i') {
            thresh_iso_highlights--;
        }
        if (key == 'I') {
            thresh_iso_highlights++;
        }
        isophotes = getIsophotes("", image, thresh_iso_highlights, false);
        cv::imshow("Hedcut Demo - Detected Isophotes", isophotes);
    }
}
// 4) accept isohpotes and begin isophote extraction
ISOPHOTE_EXTRACTION : {
    std::cout << "Beginning isophote extraction proccess\n"
                 "Press:\n"
                 "   'T' / 't' to increase / decrease threshold parameter by "
                 "10 px for edge detection (initial value is 50 px)\n";

    isophotes_extracted = extractEdges("", isophotes, thresh_isophotes, false);
    cv::destroyWindow("Hedcut Demo - Detected Isophotes");
    cv::imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
    for (;;) {
        if (auto_save) {
            std::string tag = "-isophotes-" + std::to_string(thresh_isophotes);
            save(isophotes_extracted, image_path, tag);
        }
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Extracted Isophotes");
            goto ISOPHOTE_DETECTION;
        }
        if (key == 'n' || key == 'N') {
            goto OFFSET_MAP;
        }
        if (key == 'r' || key == 'R') {
            thresh_isophotes = 50;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-isophotes-" + std::to_string(thresh_isophotes);
            save(isophotes_extracted, image_path, tag);
        }
        if (key == 't') {
            thresh_isophotes -= 10;
        }
        if (key == 'T') {
            thresh_isophotes += 10;
        }
        cv::Mat map = extractEdges("", isophotes, thresh_isophotes, false);
        cv::imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
    }
}
// 5) accept isophotes and begin offsetmap generation
OFFSET_MAP : {
    std::cout
        << "Beginning offset map proccess\n"
           "Press:\n"
           "   'L' / 'l' to increase / decrease offset lane distance by 0.5 px "
           "(initial value is 6.0 px)\n";

    offset_map = fullMap("", edges, "", isophotes_extracted, l, true, false);
    offset_map_visual =
        fullMap("", edges, "", isophotes_extracted, l, false, false);
    offset_map_visual.convertTo(offset_map_visual, CV_8UC1);
    cv::destroyWindow("Hedcut Demo - Extracted Isophotes");
    cv::imshow("Hedcut Demo - Offset Map", offset_map_visual);
    for (;;) {
        if (auto_save) {
            std::string tag = "-offset-map-" + std::to_string(l);
            save(offset_map, image_path, tag);
        }
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Offset Map");
            goto ISOPHOTE_EXTRACTION;
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
            l -= 0.25;
        }
        if (key == 'L') {
            l += 0.25;
        }
        offset_map =
            fullMap("", edges, "", isophotes_extracted, l, true, false);
        offset_map_visual =
            fullMap("", edges, "", isophotes_extracted, l, false, false);
        offset_map_visual.convertTo(offset_map_visual, CV_8UC1);
        cv::imshow("Hedcut Demo - Offset Map", offset_map_visual);
    }
}
// 6) accept offset map and begin dot adjusting
ADJUST_DOTS : {
    std::cout << "Beginning dot adjusting process.\n";

    adjusted_dots = dots("", offset_map, false);
    std::cout << "Finished adjusting dots" << std::endl;
    cv::destroyWindow("Hedcut Demo - Offset Map");
    cv::imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);
    for (;;) {
        if (auto_save) {
            std::string tag = "-adjusted";
            save(adjusted_dots, image_path, tag);
        }
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Adjusted Dots");
            goto OFFSET_MAP;
        }
        if (key == 'n' || 'N') {
            goto PLACE_CIRCLES;
        }
        if (key == 's' || 'S') {
            std::string tag = "-adjusted";
            save(adjusted_dots, image_path, tag);
        }
        adjusted_dots = dots("", offset_map, false);
        std::cout << "Finished adjusting dots" << std::endl;
        cv::imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);
    }
}
// 7) accept adjusted dots and place final circles
PLACE_CIRCLES : {
    std::cout << "Beginning circle placement process.\n"
                 "Press:\n"
                 "   'D' / 'd' to increase / decrease maximum circle size by 1 "
                 "(initial value is 12 px)\n";

    rendered = placeDots("", adjusted_dots, "", image, max_size, false);
    cv::destroyWindow("Hedcut Demo - Adjusted Dots");
    cv::imshow("Hedcut Demo - Rendered", rendered);
    for (;;) {
        if (auto_save) {
            std::string tag = "-rendered-" + std::to_string(max_size);
            save(rendered, image_path, tag);
        }
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Rendered");
            goto ADJUST_DOTS;
        }
        if (key == 's' || key == 'S') {
            save(rendered, image_path, "-rendered");
        }
        if (key == 'r' || key == 'R') {
            max_size = 15;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-rendered-" + std::to_string(max_size);
            save(rendered, image_path, tag);
        }
        if (key == 'd') {
            max_size--;
        }
        if (key == 'D') {
            max_size++;
        }
        rendered = placeDots("", adjusted_dots, "", image, max_size, false);
        cv::imshow("Hedcut Demo - Rendered", rendered);
    }
}
    return EXIT_SUCCESS;
}