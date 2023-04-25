#include "caboodle.hpp"

void mouseHandler(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN || event == EVENT_RBUTTONDOWN ||
        event == EVENT_MBUTTONDOWN) {
        if (flag == 0) {
            point = Point(x, y);
            circle(img1, point, 2, Scalar(0, 0, 255), -1, 8, 0);
            pts.push_back(point);
            var++;
            if (var > 1)
                line(img1, pts[var - 2], point, Scalar(0, 0, 255), 2, 8, 0);
            imshow("Hedcut Demo - Select Details", img1);
        }
    }
}

int main(int argc, char** argv) {
    CommandLineParser parser(argc, argv, "{@input   ||input image}");

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
    String image_path = parser.get<String>("@input");
    image = read(image_path, image);
    if (image.empty()) {
        printf("Error opening image: %s\n", image_path.c_str());
        return EXIT_FAILURE;
    }
    if (image.channels() == 4) {
        cvtColor(image, image, COLOR_RGBA2RGB);
    }

SET_UP : {
    std::cout << "\nBeginning hedcut generation proccess\n"
                 "Press:\n"
                 "\t'A' / 'a' to auto save on each step\n";
    for (;;) {
        imshow("Hedcut Demo - Initial Input", image);
        char key = (char)waitKey(0);
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
            destroyWindow("Hedcut Demo - Initial Input");
            goto EDGE_EXTRACTION;
        }
    }
}
// 2) loop over edge detection, allow tuning of threshold parameter
EDGE_EXTRACTION : {
    std::cout << "\nBeginning edge detection proccess\n\n"
                 "Press:\n"
                 "\tT' / 't'to increase / decrease threshold parameter by 25 "
                 "px for edge detection (initial "
                 "value is 300 px)\n";
    edges = extractEdges(image_path, image, thresh_edges, false);
    imshow("Hedcut Demo - Extracted Edges", edges);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-edges-" + std::to_string(thresh_edges);
            save(edges, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Extracted Edges");
            goto SET_UP;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Extracted Edges");
            goto POSTERIZE;
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
        imshow("Hedcut Demo - Extracted Edges", edges);
    }
}
// 3) accept edge image and posterize input image
POSTERIZE : {
    std::cout << "\nBeginning posterization proccess\n\n"
                 "Press:\n"
                 "\t'P' / 'p' to increase / decrease number of bins for "
                 "posterization (initial value is 5)\n ";
    posterized = posterize(image_path, image, bins, false);
    imshow("Hedcut Demo - Posterized", posterized);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-posterized-" + std::to_string(bins);
            save(posterized, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Posterized");
            goto EDGE_EXTRACTION;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Posterized");
            goto ISOPHOTE_DETECTION;
        }
        if (key == 'r' || key == 'R') {
            bins = BINS;
        }
        if (key == 'p') {
            if (bins != 1) {
                bins--;
            }
        }
        if (key == 'P') {
            bins++;
        }
        posterized = posterize(image_path, image, bins, false);
        imshow("Hedcut Demo - Posterized", posterized);
    }
}
// 4) accept posterized image and select isophotes
ISOPHOTE_DETECTION : {
    std::cout << "\nBeginning isophote detection proccess\n\n"
                 "Press:\n"
                 "\t'I' / 'i' to increase / decrease fraction of isophotes "
                 "taken to 1/n (initial value is 1/5)\n ";
    isophotes =
        getIsophotes(image_path, posterized, thresh_iso_highlights, false);
    imshow("Hedcut Demo - Detected Isophotes", isophotes);
    for (;;) {
        std::cout << thresh_iso_highlights << std::endl;
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag =
                "-detected-isophotes-" + std::to_string(thresh_iso_highlights);
            save(isophotes, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Detected Isophotes");
            goto POSTERIZE;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Detected Isophotes");
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
            getIsophotes(image_path, posterized, thresh_iso_highlights, false);
        imshow("Hedcut Demo - Detected Isophotes", isophotes);
    }
}
// 5) accept isohpotes and begin isophote extraction
ISOPHOTE_EXTRACTION : {
    std::cout << "\nBeginning isophote extraction proccess\n"
                 "Press:\n"
                 "\t'T' / 't' to increase / decrease threshold parameter by "
                 "10 px for edge detection (initial value is 50 px)\n";

    isophotes_extracted =
        extractEdges(image_path, isophotes, thresh_isophotes, false);
    imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-isophotes-" + std::to_string(thresh_isophotes);
            save(isophotes_extracted, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Extracted Isophotes");
            goto ISOPHOTE_DETECTION;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Extracted Isophotes");
            goto DETAIL_SELECTION;
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
        imshow("Hedcut Demo - Extracted Isophotes", isophotes_extracted);
    }
}
// 6) select regions that will be more detailed
DETAIL_SELECTION : {
    std::cout << "\nBeginning detail selection proccess\n"
                 "Press:\n"
                 "\tany mouse button to set points to create mask shape\n"
                 "\t'a' / 'A' to add a new section\n"
                 "\t'D' / 'd' to select detail area\n";
    ;
    destroyWindow("Hedcut Demo - Extracted Isophotes");
    mouse_src = image;
    img1 = mouse_src.clone();
    namedWindow("Hedcut Demo - Select Details", WINDOW_AUTOSIZE);
    setMouseCallback("Hedcut Demo - Select Details", mouseHandler, NULL);
    imshow("Hedcut Demo - Select Details", mouse_src);
    bool finalized = false;
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-details";
            save(mouse_src, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            // reset variables
            pts.clear();
            sections.clear();
            var = 0;
            drag = 0;
            flag = 0;
            destroyWindow("Hedcut Demo - Select Details");
            goto ISOPHOTE_EXTRACTION;
        }
        if (key == 'n' || key == 'N') {
            if (!finalized) {
                if (!pts.empty()) {
                    sections.push_back(pts);
                    pts.clear();
                }
                flag = 1;
                img1 = mouse_src.clone();
                for (std::vector<Point> s : sections) {
                    polylines(img1, s, 1, Scalar(0, 0, 0), 2, 8, 0);
                }
                flag = var;
                final = Mat::zeros(mouse_src.size(), CV_8UC3);
                mask = Mat::zeros(mouse_src.size(), CV_8UC1);
                fillPoly(mask, sections, Scalar(255, 255, 255), 8, 0);
                bitwise_and(mouse_src, mouse_src, final, mask);
                imshow("Hedcut Demo - Select Details", final);
            }
            destroyWindow("Hedcut Demo - Select Details");
            goto OFFSET_MAP;
        }
        if (key == 'a' || key == 'A') {
            if (!pts.empty()) {
                // save current section
                sections.push_back(pts);
            }
            // reset variables
            flag = 0;
            pts.clear();
            var = 0;
        }
        if (key == 'd' || key == 'D') {
            if (!pts.empty()) {
                sections.push_back(pts);
                pts.clear();
            }
            flag = 1;
            img1 = mouse_src.clone();
            for (std::vector<Point> s : sections) {
                polylines(img1, s, 1, Scalar(0, 0, 0), 2, 8, 0);
            }
            flag = var;
            final = Mat::zeros(mouse_src.size(), CV_8UC3);
            mask = Mat::zeros(mouse_src.size(), CV_8UC1);
            fillPoly(mask, sections, Scalar(255, 255, 255), 8, 0);
            bitwise_and(mouse_src, mouse_src, final, mask);
            imshow("Hedcut Demo - Select Details", final);
            finalized = true;
        }
        if (key == 'r' || key == 'R') {
            pts.clear();
            sections.clear();
            var = 0;
            drag = 0;
            flag = 0;
            finalized = false;
            img1 = mouse_src.clone();
            imshow("Hedcut Demo - Select Details", mouse_src);
        }
    }
}
// 7) accept isophotes and begin offsetmap generation
OFFSET_MAP : {
    assert(!final.empty());
    std::cout << "\nBeginning offset map proccess\n"
                 "Press:\n"
                 "\t'L' / 'l' to increase / decrease offset lane distance "
                 "by 1 px "
                 "(initial value is 6.0 px)\n";

    // 7.5) show distance map
    distances =
        distanceMap(image_path, edges, image_path, isophotes_extracted, false);
    Mat distancesToShow = distances.clone();
    distancesToShow.convertTo(distancesToShow, CV_8UC1);
    imshow("Hedcut Demo - Distance Map", distancesToShow);
    waitKey(0);
    destroyWindow("Hedcut Demo - Distance Map");
    offset_map =
        offsetMap(image_path, distances, image_path, mask, l, false, true);
    offset_map_visual =
        offsetMap(image_path, distances, image_path, mask, l, false, false);
    offset_map_visual.convertTo(offset_map_visual, CV_8UC1);
    imshow("Hedcut Demo - Offset Map", offset_map_visual);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-offset-map-" + std::to_string(l);
            save(offset_map_visual, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Offset Map");
            goto DETAIL_SELECTION;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Offset Map");
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
        offset_map =
            offsetMap(image_path, distances, image_path, mask, l, false, true);
        offset_map_visual =
            offsetMap(image_path, distances, image_path, mask, l, false, false);
        offset_map_visual.convertTo(offset_map_visual, CV_8UC1);
        imshow("Hedcut Demo - Offset Map", offset_map_visual);
    }
}
// 8) place dots
PLACE_DOTS : {
    std::cout << "\nBeginning dot placement map proccess\n"
                 "Press:\n"
                 "\t'D' / 'd' to increase / decrease space between dots "
                 "distance by 1 px "
                 "(initial value is 6.0 px)\n";

    d = l;
    initial_dots =
        placeSeeds(image_path, offset_map, image_path, mask, d, false);
    std::cout << "Finished placing dots" << std::endl;
    imshow("Hedcut Demo - Initial Dots", initial_dots);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-initial-dots";
            save(initial_dots, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Initial Dots");
            goto OFFSET_MAP;
        }
        if (key == 'd') {
            d--;
        }
        if (key == 'D') {
            d++;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Initial Dots");
            goto ADJUST_DOTS;
        }
        if (key == 'r' || key == 'R') {
            d = L;
        }
        initial_dots =
            placeSeeds(image_path, offset_map, image_path, mask, d, false);
        std::cout << "Finished placing seed dots" << std::endl;
        imshow("Hedcut Demo - Initial Dots", initial_dots);
    }
}
// 9) accept initial dots and begin dot adjusting
ADJUST_DOTS : {
    std::cout << "\nBeginning dot adjusting process.\n";

    adjusted_dots =
        dots(image_path, offset_map, image_path, initial_dots, l, false);
    std::cout << "Finished adjusting dots" << std::endl;
    imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-adjusted";
            save(adjusted_dots, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Adjusted Dots");
            goto PLACE_DOTS;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Adjusted Dots");
            goto POSTERIZE_NEGATIVE;
        }
        adjusted_dots =
            dots(image_path, offset_map, image_path, initial_dots, d, false);
        std::cout << "Finished adjusting dots" << std::endl;
        imshow("Hedcut Demo - Adjusted Dots", adjusted_dots);
    }
}

// 10) posterize image for negative space selection
POSTERIZE_NEGATIVE : {
    std::cout << "\nBeginning posterization proccess for negative space\n\n"
                 "Press:\n"
                 "\t'P' / 'p' to increase / decrease number of bins for "
                 "posterization (initial value is 5)\n ";
    negative_posterized = posterize(image_path, image, negative_bins, false);
    imshow("Hedcut Demo - Negative Posterized", negative_posterized);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag =
                "-negative-posterized-" + std::to_string(negative_bins);
            save(negative_posterized, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Negative Posterized");
            goto ADJUST_DOTS;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Negative Posterized");
            goto CHOOSE_NEGATIVE;
        }
        if (key == 'r' || key == 'R') {
            negative_bins = NEGATIVE_SPACE_BINS;
        }
        if (key == 'p') {
            if (negative_bins != 1) {
                negative_bins--;
            }
        }
        if (key == 'P') {
            negative_bins++;
        }
        negative_posterized =
            posterize(image_path, image, negative_bins, false);
        imshow("Hedcut Demo - Negative Posterized", negative_posterized);
    }
}
// 11) accept adjusted dots and choose areas to not place circles
CHOOSE_NEGATIVE : {
    std::cout << "\nBeginning negative space detection proccess.\nWhite areas "
                 "will not contain dots in final image.\n\n"
                 "Press:\n"
                 "\t'I' / 'i' to increase / decrease fraction of isophotes "
                 "taken to 1/n (initial value is 1/5)\n ";
    negative_space = getIsophotes(image_path, negative_posterized,
                                  thresh_negative_space, false);
    imshow("Hedcut Demo - Negative Space", negative_space);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag =
                "-negative-space-" + std::to_string(thresh_negative_space);
            save(negative_space, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Negative Space");
            goto POSTERIZE_NEGATIVE;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Negative Space");
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
        negative_space = getIsophotes(image_path, negative_posterized,
                                      thresh_negative_space, false);
        imshow("Hedcut Demo - Negative Space", negative_space);
    }
}
// 12) accept adjusted dots and place final circles
PLACE_CIRCLES : {
    std::cout << "\nBeginning circle placement process.\n"
                 "Press:\n"
                 "\t'D' / 'd' to increase / decrease maximum circle size by 1 "
                 "(initial value is 12 px)\n";

    float density = 1 / float(l * l);
    max_size = sqrt((1 / density));
    rendered = placeDotsNegativeSpace(image_path, adjusted_dots, image_path,
                                      image, image_path, negative_space,
                                      image_path, mask, max_size, false);
    imshow("Hedcut Demo - Rendered", rendered);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-rendered-" + std::to_string(max_size);
            save(rendered, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Rendered");
            goto CHOOSE_NEGATIVE;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Rendered");
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
        rendered = placeDotsNegativeSpace(image_path, adjusted_dots, image_path,
                                          image, image_path, negative_space,
                                          image_path, mask, max_size, false);
        imshow("Hedcut Demo - Rendered", rendered);
    }
}
// 13) accept rendered dots, find image outline
OUTLINE : {
    std::cout << "\nBeginning outline detection proccess\n\n"
                 "Press:\n"
                 "\t'T' / 't'to increase / decrease threshold parameter by 25 "
                 "px for edge detection (initial "
                 "value is 500 px)\n";
    outline = extractEdges(image_path, image, thresh_outline, false);
    imshow("Hedcut Demo - Outline", outline);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-outline-" + std::to_string(thresh_edges);
            save(edges, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Outline");
            goto PLACE_CIRCLES;
        }
        if (key == 'n' || key == 'N') {
            destroyWindow("Hedcut Demo - Outline");
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
        imshow("Hedcut Demo - Outline", outline);
    }
}
// 14) combine final rendering
FINAL_RENDERING : {
    std::cout << "\nFinalizing rendering\n\n";

    destroyWindow("Hedcut Demo - Outline");

    resize(outline, enlarged_outline, Size(), scale, scale);
    bitwise_and(rendered, enlarged_outline, final_rendering);
    imshow("Hedcut Demo - Final Rendering", final_rendering);
    for (;;) {
        char key = (char)waitKey(0);
        if (auto_save || key == 's' || key == 'S') {
            std::string tag = "-final-rendering";
            save(final_rendering, image_path, tag);
        }
        if (key == 27) {
            return EXIT_SUCCESS;
        }
        if (key == 'b' || key == 'B') {
            destroyWindow("Hedcut Demo - Final Rendering");
            goto OUTLINE;
        }
    }
}

    return EXIT_SUCCESS;
}