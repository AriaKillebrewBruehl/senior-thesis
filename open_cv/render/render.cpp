#include "render.hpp"

int main(int argc, char** argv) {
    cv::CommandLineParser parser(argc, argv, "{@input   ||input image}");

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
                 "  'A' / 'a' to auto save on each step\n";
    for (;;) {
        cv::imshow("Hedcut Demo - Initial Input", image);
        char key = (char)cv::waitKey(0);
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'a' || key == 'A') {
            auto_save = !auto_save;
            std::cout << "AUTO SAVE IS";
            if (auto_save) {
                std::cout << " ON. Hit 'a' to turn off.";
            } else {
                std::cout << " OFF. Hit 'a' to turn on.";
            }
            std::cout << std::endl;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Initial Input");
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
    edges = extractEdges(image_path, image, thresh_edges, false);

    cv::imshow("Hedcut Demo - Extracted Edges", edges);
    for (;;) {
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
        edges = extractEdges(image_path, image, thresh_edges, false);
        cv::imshow("Hedcut Demo - Extracted Edges", edges);
    }
}

// 3) accept edge image and begin isophotes detection
ISOPHOTE_DETECTION : {
    std::cout << "\nBeginning isophote detection proccess\n\n"
                 "Press:\n"
                 "\t'I' / 'i' to increase / decrease fraction of isophotes "
                 "taken (initial value is top 1 section)\n ";
    isophotes = getIsophotes(image_path, image, thresh_iso_highlights, false);
    cv::imshow("Hedcut Demo - Detected Isophotes", isophotes);
    for (;;) {
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
        isophotes =
            getIsophotes(image_path, image, thresh_iso_highlights, false);
        cv::imshow("Hedcut Demo - Detected Isophotes", isophotes);
    }
}
// 4) accept isohpotes and begin isophote extraction
ISOPHOTE_EXTRACTION : {
    std::cout << "\nBeginning isophote extraction proccess\n"
                 "Press:\n"
                 "\t'T' / 't' to increase / decrease threshold parameter by "
                 "10 px for edge detection (initial value is 50 px)\n";

    isophotes_extracted =
        extractEdges(image_path, isophotes, thresh_isophotes, false);
    cv::imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
    for (;;) {
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
           "\t'L' / 'l' to increase / decrease offset lane distance by 1 px "
           "(initial value is 6.0 px)\n";

    distances =
        distanceMap(image_path, edges, image_path, isophotes_extracted, false);
    offset_map = offsetMap(image_path, distances, l, false, true);
    offset_map_visual = offsetMap(image_path, distances, l, false, false);
    offset_map_visual.convertTo(offset_map_visual, CV_8UC1);
    cv::imshow("Hedcut Demo - Offset Map", offset_map_visual);
    for (;;) {
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
                 "\t'D' / 'd' to increase / decrease space between dots "
                 "distance by 1 px "
                 "(initial value is 6.0 px)\n";

    d = l;
    initial_dots = placeSeedsAdjusted(image_path, offset_map, image_path,
                                      distances, d, false);
    std::cout << "Finished placing dots" << std::endl;
    cv::imshow("Hedcut Demo - Initial Dots", initial_dots);
    for (;;) {
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
        initial_dots = placeSeedsAdjusted(image_path, offset_map, image_path,
                                          distances, d, false);
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
    cv::imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);
    for (;;) {
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
            goto CHOOSE_NEGATIVE;
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
                 "\t'I' / 'i' to increase / decrease fraction of isophotes "
                 "taken to 1/n (initial value is 1/5)\n ";
    negative_space =
        getIsophotes(image_path, image, thresh_negative_space, false);
    cv::imshow("Hedcut Demo - Negative Space", negative_space);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag =
                "-negative-space-" + std::to_string(thresh_negative_space);
            save(negative_space, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Negative Space");
            goto ADJUST_DOTS;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Negative Space");
            goto PLACE_CIRCLES;
        }
        if (key == 'r' || key == 'R') {
            thresh_negative_space = NEGATIVE_SPACE_THRESH;
        }
        if (key == 'i') {
            if (thresh_negative_space != 1) {
                thresh_negative_space--;
            }
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
                 "\t'D' / 'd' to increase / decrease maximum circle size by 1 "
                 "(initial value is 12 px)\n";

    rendered =
        placeDotsNegativeSpace(image_path, adjusted_dots, image_path, image,
                               image_path, negative_space, max_size, false);

    cv::imshow("Hedcut Demo - Rendered", rendered);
    for (;;) {
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
            goto CHOOSE_NEGATIVE;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Rendered");
            goto OUTLINE;
        }
        if (key == 'r' || key == 'R') {
            max_size = MAX_SIZE;
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
// 10) accept rendered dots, find image outline
OUTLINE : {
    std::cout << "\nBeginning outline detection proccess\n\n"
                 "Press:\n"
                 "\t'T' / 't'to increase / decrease threshold parameter by 25 "
                 "px for edge detection (initial "
                 "value is 500 px)\n";

    outline = extractEdges(image_path, image, thresh_outline, false);
    cv::imshow("Hedcut Demo - Outline", outline);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-outline-" + std::to_string(thresh_edges);
            save(edges, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Outline");
            goto PLACE_CIRCLES;
        }
        if (key == 'n' || key == 'N') {
            cv::destroyWindow("Hedcut Demo - Outline");
            goto FINAL_RENDERING;
        }
        if (key == 'r' || key == 'R') {
            thresh_outline = OUTLINE_THRESH;
        }
        if (key == 't') {
            thresh_outline -= 25;
        }
        if (key == 'T') {
            thresh_outline += 25;
        }
        outline = extractEdges(image_path, image, thresh_outline, false);
        cv::imshow("Hedcut Demo - Outline", outline);
    }
}
// 11) combine final rendering
FINAL_RENDERING : {
    std::cout << "\nFinalizing rendering\n\n";

    int scale = 6;
    cv::Mat enlarged_outline;
    cv::resize(outline, enlarged_outline, cv::Size(), scale, scale);
    cv::bitwise_and(rendered, enlarged_outline, final_rendering);
    cv::imshow("Hedcut Demo - Final Rendering", final_rendering);
    for (;;) {
        char key = (char)cv::waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-final-rendering";
            save(final_rendering, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            cv::destroyWindow("Hedcut Demo - Final Rendering");
            goto OUTLINE;
        }
    }
}

    return EXIT_SUCCESS;
}