#include "caboodle.hpp"

/*
cv::Mat caboodle(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());
    if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
    }

    // step 1: extract the edges of the image
    cv::Mat edges = extractEdges(path, image, 300, false);
    if (edges.type() != 0) {
        edges.convertTo(edges, 0);
    }

    std::cout << "extracted edges from image" << std::endl;

    // step 2: extract the isophotes of the image
    cv::Mat isophotes = extractIsophotes(path, image, 10, 5, false);
    if (isophotes.type() != 0) {
        isophotes.convertTo(isophotes, 0);
    }
    std::cout << "extracted isophotes from image" << std::endl;

    // step 3: offset map
    cv::Mat map = fullMap(path, edges, path, isophotes, 6.0, true, false);
    std::cout << "extracted offset map from image" << std::endl;

    // step 4: generate initial dot placement
    cv::Mat seeds = placeSeeds(path, map, 6.0, false);
    std::cout << "finalized initial dot placement for image" << std::endl;

    // step 5: generate final dot placement
    cv::Mat adjusted = dots(path, map, path, seeds, 6.0, false);
    std::cout << "finalized dot placement for image" << std::endl;

    // step 6: place circles!
    cv::Mat rendered = placeDots(path, adjusted, path, image, 20, true);
    std::cout << "sized dots" << std::endl;
    save(rendered, path, "-rendered");

    return rendered;
}
*/

int main(int argc, char** argv) {
    cv::CommandLineParser parser(argc, argv, "{@input   ||input image}");

    std::cout
        << "This program renders an input photograph as a hedcut drawing\n\n"
           "At any stage: \n"
           "   press 'B' / 'b' to move to previous step in hedcut "
           "process\n"
           "   press 'N' / 'n' to move to next step in hedcut "
           "process\n"
           "   press 'R' / 'r' to reset parameters to default values\n"
           "   press 'S' / 's' to save current image\n"
           "   press 'ESC' to exit program\n"
           "!!!\n"
           "PLEASE NOTE THIS IS A SLOW PROGRAM (particularly the dot adjusting "
           "step) DO NOT SPAM THE KEYS IF YOU SEE NO CHANGE\n"
           "!!!\n";

    // 1) read in input
    cv::Mat image;
    cv::String image_path = parser.get<cv::String>("@input");

    image = read(image_path, image);
    if (image.empty()) {
        printf("Error opening image: %s\n", image_path.c_str());
        return EXIT_FAILURE;
    }

    if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
    }

    // set up default parameter values
    const int EDGE_THRESH = 300;
    const int ISOS_HIGHLIGHT_THRESH = 5;
    const int ISOS_THRESH = 200;
    const int L = 6.0;
    const int NEGATIVE_SPACE_THRESH = 6;
    const int MAX_SIZE = 15;
    // setup variables
    bool auto_save = false;
    cv::Mat edges;
    int thresh_edges = EDGE_THRESH;
    cv::Mat isophotes;
    int thresh_iso_highlights = ISOS_HIGHLIGHT_THRESH;
    cv::Mat isophotes_extracted;
    int thresh_isophotes = ISOS_THRESH;
    cv::Mat offset_map;
    cv::Mat offset_map_visual;
    int l = L;
    cv::Mat initial_dots;
    int d = l;
    cv::Mat adjusted_dots;
    cv::Mat negative_space;
    int thresh_negative_space = NEGATIVE_SPACE_THRESH;
    cv::Mat rendered;
    int max_size = MAX_SIZE;

SET_UP : {
    std::cout << "\nBeginning hedcut generation proccess\n"
                 "Press:\n"
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
        if (key == 'n' || key == 'N') {
            goto EDGE_EXTRACTION;
        }
    }
}
// 2) loop over edge detection, allow tuning of threshold paramenter
EDGE_EXTRACTION : {
    std::cout << "\nBeginning edge detection proccess\n\n"
                 "Press:\n"
                 "   'T' / 't'to increase / decrease threshold parameter by 25 "
                 "px for edge detection (initial "
                 "value is 300 px)\n";

    thresh_edges = 300;
    edges = extractEdges(image_path, image, thresh_edges, false);
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
            thresh_edges = EDGE_THRESH;
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
        edges = extractEdges(image_path, image, thresh_edges, false);
        cv::imshow("Hedcut Demo - Extracted Edges", edges);
    }
}
// 3) accept edge image and begin isophotes detection
ISOPHOTE_DETECTION : {
    std::cout << "\nBeginning isophote detection proccess\n\n"
                 "Press:\n"
                 "   'I' / 'i' to increase / decrease fraction of isophotes "
                 "taken to 1/n (initial value is 1/5)\n ";
    isophotes = getIsophotes(image_path, image, thresh_iso_highlights, false);
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
            thresh_iso_highlights = ISOS_HIGHLIGHT_THRESH;
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
        isophotes =
            getIsophotes(image_path, image, thresh_iso_highlights, false);
        cv::imshow("Hedcut Demo - Detected Isophotes", isophotes);
    }
}
// 4) accept isohpotes and begin isophote extraction
ISOPHOTE_EXTRACTION : {
    std::cout << "\nBeginning isophote extraction proccess\n"
                 "Press:\n"
                 "   'T' / 't' to increase / decrease threshold parameter by "
                 "10 px for edge detection (initial value is 50 px)\n";

    isophotes_extracted =
        extractEdges(image_path, isophotes, thresh_isophotes, false);
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
            thresh_isophotes = ISOS_THRESH;
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
        isophotes_extracted =
            extractEdges(image_path, isophotes, thresh_isophotes, false);
        cv::imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
    }
}
// 5) accept isophotes and begin offsetmap generation
OFFSET_MAP : {
    std::cout
        << "\nBeginning offset map proccess\n"
           "Press:\n"
           "   'L' / 'l' to increase / decrease offset lane distance by 1 px "
           "(initial value is 6.0 px)\n";

    offset_map = fullMap(image_path, edges, image_path, isophotes_extracted, l,
                         true, false);
    offset_map_visual = fullMap(image_path, edges, image_path,
                                isophotes_extracted, l, false, false);
    offset_map_visual.convertTo(offset_map_visual, CV_8UC1);
    cv::destroyWindow("Hedcut Demo - Extracted Isophotes");
    cv::imshow("Hedcut Demo - Offset Map", offset_map_visual);
    for (;;) {
        if (auto_save) {
            std::string tag = "-offset-map-" + std::to_string(l);
            save(offset_map_visual, image_path, tag);
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
            goto PLACE_DOTS;
        }
        if (key == 'r' || key == 'R') {
            l = L;
        }
        if (key == 's' || key == 'S') {
            std::string tag = "-offset-map-" + std::to_string(l);
            save(offset_map_visual, image_path, tag);
        }
        if (key == 'l') {
            l -= 1.0;
        }
        if (key == 'L') {
            l += 1.0;
        }
        offset_map = fullMap(image_path, edges, image_path, isophotes_extracted,
                             l, true, false);
        offset_map_visual = fullMap(image_path, edges, image_path,
                                    isophotes_extracted, l, false, false);
        offset_map_visual.convertTo(offset_map_visual, CV_8UC1);
        cv::imshow("Hedcut Demo - Offset Map", offset_map_visual);
    }
}
// 6) place dots
PLACE_DOTS : {
    std::cout << "\nBeginning dot placement map proccess\n"
                 "Press:\n"
                 "   'D' / 'd' to increase / decrease space between dots "
                 "distance by 1 px "
                 "(initial value is 6.0 px)\n";

    d = l;
    initial_dots = placeSeeds(image_path, offset_map, d, false);
    std::cout << "Finished placing dots" << std::endl;
    cv::destroyWindow("Hedcut Demo - Offset Map");
    cv::imshow("Hedcut Demo - Initial Dots", initial_dots);
    for (;;) {
        if (auto_save) {
            std::string tag = "-initial-dots";
            save(initial_dots, image_path, tag);
        }
        char key = (char)cv::waitKey(0);
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
            goto ADJUST_DOTS;
        }
        if (key == 'r' || key == 'R') {
            d = L;
        }
        if (key == 's' || 'S') {
            std::string tag = "-initial-dots";
            save(initial_dots, image_path, tag);
        }
        initial_dots = placeSeeds(image_path, offset_map, d, false);
        std::cout << "Finished placing seed dots" << std::endl;
        cv::imshow("Hedcut Demo - Initial Dots", initial_dots);
    }
}
// 7) accept initial dots and begin dot adjusting
ADJUST_DOTS : {
    std::cout << "\nBeginning dot adjusting process.\n";

    adjusted_dots =
        dots(image_path, offset_map, image_path, initial_dots, l, false);
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
            goto PLACE_DOTS;
        }
        if (key == 'n' || key == 'N') {
            goto CHOOSE_NEGATIVE;
        }
        if (key == 's' || 'S') {
            std::string tag = "-adjusted";
            save(adjusted_dots, image_path, tag);
        }
        adjusted_dots =
            dots(image_path, offset_map, image_path, initial_dots, d, false);
        std::cout << "Finished adjusting dots" << std::endl;
        cv::imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);
    }
}
// 8) accept adjusted dots and choose areas to not place circles
CHOOSE_NEGATIVE : {
    std::cout << "\nBeginning negative space detection proccess.\nWhite areas "
                 "will not contain dots in final image.\n\n"
                 "Press:\n"
                 "   'I' / 'i' to increase / decrease fraction of isophotes "
                 "taken to 1/n (initial value is 1/5)\n ";
    negative_space =
        getIsophotes(image_path, image, thresh_negative_space, false);
    cv::destroyWindow("Hedcut Demo - Adjusted Dots");
    cv::imshow("Hedcut Demo - Negative Space", negative_space);
    for (;;) {
        if (auto_save) {
            std::string tag =
                "-negative-space-" + std::to_string(thresh_negative_space);
            save(negative_space, image_path, tag);
        }
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Negative Space");
            goto ADJUST_DOTS;
        }
        if (key == 'n' || key == 'N') {
            goto PLACE_CIRCLES;
        }
        if (key == 'r' || key == 'R') {
            thresh_negative_space = NEGATIVE_SPACE_THRESH;
        }
        if (key == 's' || key == 'S') {
            std::string tag =
                "-negative-space-" + std::to_string(thresh_negative_space);
            save(negative_space, image_path, tag);
        }
        if (key == 'i') {
            thresh_negative_space--;
        }
        if (key == 'I') {
            thresh_negative_space++;
        }
        negative_space =
            getIsophotes(image_path, image, thresh_negative_space, false);
        cv::imshow("Hedcut Demo - Negative Space", negative_space);
    }
}
// 9) accept adjusted dots and place final circles
PLACE_CIRCLES : {
    std::cout << "\nBeginning circle placement process.\n"
                 "Press:\n"
                 "   'D' / 'd' to increase / decrease maximum circle size by 1 "
                 "(initial value is 12 px)\n";

    rendered =
        placeDotsNegativeSpace(image_path, adjusted_dots, image_path, image,
                               image_path, negative_space, max_size, false);
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
        rendered =
            placeDotsNegativeSpace(image_path, adjusted_dots, image_path, image,
                                   image_path, negative_space, max_size, false);
        cv::imshow("Hedcut Demo - Rendered", rendered);
    }
}
    return EXIT_SUCCESS;
}