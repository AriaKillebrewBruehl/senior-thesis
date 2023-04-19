#include "caboodle.hpp"

int main(int argc, char** argv) {
    cv::CommandLineParser parser(
        argc, argv,
        "{@input   ||input image}"
        "{go G g   |false|process image with no breaks (hit 'G' to process "
        "with no breaks at runtime)}");

    std::cout
        << "This program renders an input photograph as a hedcut drawing\n\n"
           "At any stage: \n"
           "\tpress 'B' / 'b' to move to previous step in hedcut "
           "process\n"
           "\tpress 'N' / 'n' to move to next step in hedcut "
           "process\n"
           "\tpress 'R' / 'r' to reset parameters to default values\n"
           "\tpress 'S' / 's' to save current image\n"
           "\tpress 'ESC' to exit program\n"
           "!!!\n"
           "PLEASE NOTE THIS IS A SLOW PROGRAM (particularly the dot adjusting "
           "step) DO NOT SPAM THE KEYS IF YOU SEE NO CHANGE\n"
           "!!!\n";

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

SET_UP : {
    std::cout << "\nBeginning hedcut generation proccess\n"
                 "Press:\n"
                 "\t'A' / 'a' to auto save on each step\n";
    for (;;) {
        cv::imshow("Hedcut Demo - Initial Input", image);
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'a' || key == 'A') {
            auto_save = true;
        }
        if (key == 'n' || key == 'N') {
            goto EDGE_EXTRACTION;
        }
    }
}
// 2) loop over edge detection, allow tuning of threshold paramenter
EDGE_EXTRACTION : {
    std::cout << "\nBeginning edge detection proccess\n\n"
                 "Press:\n"
                 "\t'T' / 't'to increase / decrease threshold parameter by 25 "
                 "px for edge detection (initial "
                 "value is 300 px)\n";

    thresh_edges = EDGE_THRESH;
    cv::destroyWindow("Hedcut Demo - Initial Input");
    for (;;) {
        edges = extractEdges(image_path, image, thresh_edges, false);
        cv::imshow("Hedcut Demo - Extracted Edges", edges);

        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-edges-" + std::to_string(thresh_edges);
            save(edges, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Extracted Edges");
            goto SET_UP;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Extracted Edges");
            goto ISOPHOTE_DETECTION;
        }
        if (key == 'r' || key == 'R') {
            thresh_edges = EDGE_THRESH;
        }
        if (key == 't') {
            thresh_edges -= 25;
        }
        if (key == 'T') {
            thresh_edges += 25;
        }
    }
}
// 3) accept edge image and begin isophotes detection
ISOPHOTE_DETECTION : {
    std::cout << "\nBeginning isophote detection proccess\n\n"
                 "Press:\n"
                 "\t'I' / 'i' to increase / decrease fraction of isophotes "
                 "taken to 1/n (initial value is 1/5)\n ";

    for (;;) {
        isophotes =
            getIsophotes(image_path, image, thresh_iso_highlights, false);
        cv::imshow("Hedcut Demo - Detected Isophotes", isophotes);

        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag =
                "-detected-isophotes-" + std::to_string(thresh_iso_highlights);
            save(isophotes, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Detected Isophotes");
            goto EDGE_EXTRACTION;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Detected Isophotes");
            goto ISOPHOTE_EXTRACTION;
        }
        if (key == 'r' || key == 'R') {
            thresh_iso_highlights = ISOS_HIGHLIGHT_THRESH;
        }
        if (key == 'i') {
            if (thresh_iso_highlights != 1) {
                thresh_iso_highlights--;
            }
        }
        if (key == 'I') {
            thresh_iso_highlights++;
        }
    }
}
// 4) accept isohpotes and begin isophote extraction
ISOPHOTE_EXTRACTION : {
    std::cout << "\nBeginning isophote extraction proccess\n"
                 "Press:\n"
                 "\t'T' / 't' to increase / decrease threshold parameter by "
                 "10 px for edge detection (initial value is 50 px)\n";

    for (;;) {
        isophotes_extracted =
            extractEdges(image_path, isophotes, thresh_isophotes, false);
        cv::imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-isophotes-" + std::to_string(thresh_isophotes);
            save(isophotes_extracted, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Extracted Isophotes");
            goto ISOPHOTE_DETECTION;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Extracted Isophotes");
            goto OFFSET_MAP;
        }
        if (key == 'r' || key == 'R') {
            thresh_isophotes = ISOS_THRESH;
        }
        if (key == 't') {
            thresh_isophotes -= 10;
        }
        if (key == 'T') {
            thresh_isophotes += 10;
        }
    }
}
// 5) accept isophotes and begin offsetmap generation
OFFSET_MAP : {
    std::cout
        << "\nBeginning offset map proccess\n"
           "Press:\n"
           "\t'L' / 'l' to increase / decrease offset lane distance by 1 px "
           "(initial value is 6.0 px)\n";

    for (;;) {
        offset_map = fullMap(image_path, edges, image_path, isophotes_extracted,
                             l, true, false);
        offset_map_visual = fullMap(image_path, edges, image_path,
                                    isophotes_extracted, l, false, false);
        offset_map_visual.convertTo(offset_map_visual, CV_8UC1);

        cv::imshow("Hedcut Demo - Offset Map", offset_map_visual);
        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-offset-map-" + std::to_string(l);
            save(offset_map_visual, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Offset Map");
            goto ISOPHOTE_EXTRACTION;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Offset Map");
            goto PLACE_DOTS;
        }
        if (key == 'r' || key == 'R') {
            l = L;
        }
        if (key == 'l') {
            l -= 1.0;
        }
        if (key == 'L') {
            l += 1.0;
        }
    }
}
// 6) place dots
PLACE_DOTS : {
    std::cout << "\nBeginning dot placement map proccess\n"
                 "Press:\n"
                 "\t'D' / 'd' to increase / decrease space between dots "
                 "distance by 1 px "
                 "(initial value is 6.0 px)\n";

    d = l;
    for (;;) {
        initial_dots = placeSeeds(image_path, offset_map, d, false);
        std::cout << "Finished placing dots" << std::endl;
        cv::imshow("Hedcut Demo - Initial Dots", initial_dots);

        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-initial-dots";
            save(initial_dots, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Initial Dots");
            goto OFFSET_MAP;
        }
        if (key == 'd') {
            d--;
        }
        if (key == 'D') {
            d++;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Initial Dots");
            goto ADJUST_DOTS;
        }
        if (key == 'r' || key == 'R') {
            d = L;
        }
    }
}
// 7) accept initial dots and begin dot adjusting
ADJUST_DOTS : {
    std::cout << "\nBeginning dot adjusting process.\n";
    for (;;) {
        adjusted_dots =
            dots(image_path, offset_map, image_path, initial_dots, l, false);
        std::cout << "Finished adjusting dots" << std::endl;
        cv::imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);

        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-adjusted";
            save(adjusted_dots, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Adjusted Dots");
            goto PLACE_DOTS;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Adjusted Dots");
            goto PLACE_CIRCLES;
        }
    }
}
// 8) accept adjusted dots and place final circles
PLACE_CIRCLES : {
    std::cout << "\nBeginning circle placement process.\n"
                 "Press:\n"
                 "\t'D' / 'd' to increase / decrease maximum circle size by 1 "
                 "(initial value is 12 px)\n"
                 "\t'ESC' to exit program (make sure to save first)\n";
    for (;;) {
        rendered = placeDots(image_path, adjusted_dots, image_path, image,
                             max_size, false);
        cv::imshow("Hedcut Demo - Rendered", rendered);

        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-rendered-" + std::to_string(max_size);
            save(rendered, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Rendered");
            goto ADJUST_DOTS;
        }
        if (key == 'r' || key == 'R') {
            max_size = MAX_SIZE;
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
    }
}
    return EXIT_SUCCESS;
}