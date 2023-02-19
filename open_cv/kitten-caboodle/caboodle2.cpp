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
    cv::Mat map = fullMap("", edges, "", isophotes, false);
    std::cout << "extracted offset map from image" << std::endl;

    // step 4: generate final dot placement
    cv::Mat adjusted = dots("", map, false);
    std::cout << "finalized dot placement for image" << std::endl;

    // step 5: place circles!
    cv::Mat rendered = placeDots("", adjusted, "", image, true);
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
    const cv::String window1 = "Hedcut Demo - Initial Input";
    bool go = parser.get<bool>("go");
    cv::String image_path = parser.get<cv::String>("@input");

    image = read(image_path, image);
    if (image.empty()) {
        printf("Error opening image: %s\n", image_path.c_str());
        return EXIT_FAILURE;
    }

    {
        cv::imshow(window1, image);
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'g' || key == 'G' || go) {
            cv::Mat rendered = caboodle(image_path, image, true);
            cv::imshow(window1, rendered);
            std::cout << "\nPress any key to exit program.\n";
            char key2 = (char)cv::waitKey(0);
            if (key2) {
                return EXIT_SUCCESS;
            }
        }
        if (key == 'n' || 'N') {
            goto EDGE_EXTRACTION;
        }
    }

// 2) loop over edge detection, allow tuning of threshold paramenter
EDGE_EXTRACTION:
    std::cout
        << "Beginning edge detection proccess\nPress 'T' / 't'to increase / "
           "decrease threshold parameter by 25 px for edge detection (initial "
           "value is "
           "300 px).\nPress 'N' / 'n' to move to isophote detection "
           "step.\nPress 'ESC' to exit program.\nPress 'R' to reset values at "
           "any step";

    cv::Mat edges;
    int thresh = 300;
    for (;;) {
        edges = extractEdges("", image, thresh, false);
        cv::imshow(window1, edges);
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'n' || 'N') {
            goto ISOPHOTE_DETECTION;
        }
        if (key == 'r' || 'R') {
            thresh = 300;
        }
        if (key == 't') {
            thresh -= 25;
        }
        if (key == 'T') {
            thresh += 25;
        }
    }

// 3) accept edge image and being isophotes, allow tuning of portion of
ISOPHOTE_DETECTION:
    std::cout
        << "Beginning isophote detection proccess\nPress 'I' / 'i' to increase "
           "/ decrease fraction of isophotes taken to 1/n (initial "
           "value is 1/5).\nPress 'N' / 'n' to move to isophote extraction "
           "step.\nPress 'ESC' to exit program.\nPress 'R' to reset values at "
           "any step";
    cv::Mat isophotes;
    int thresh = 5;
    for (;;) {
        isophotes = getIsophotes("", edges, thresh, false);
        cv::imshow(window1, isophotes);
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'n' || 'N') {
            goto ISOPHOTE_EXTRACTION;
        }
        if (key == 'r' || 'R') {
            thresh = 5;
        }
        if (key == 'i') {
            thresh--;
        }
        if (key == 'I') {
            thresh++;
        }
    }

ISOPHOTE_EXTRACTION:
    // isophotes taken and threshold parament

    // 3) accept isophotes and generate offset map

    // 4) generate final dot placement

    // generate dots, pick max dot size
    // dot cv::CommandLineParser parser(
    //     argc, argv,
    // )
    return EXIT_SUCCESS;
}