#include "caboodle.hpp"
cv::Mat caboodle(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());
    if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
    }

    std::cout << "begining fully automated hedcut detection" << std::endl;
    // step 1: extract the edges of the image
    cv::Mat edges = extractEdges(path, image, 300, false);
    if (edges.type() != 0) {
        edges.convertTo(edges, 0);
    }

    std::cout << "extracted edges from image" << std::endl;

    // step 2: extract the isophotes of the image
    cv::Mat isophotes = extractIsophotes(path, image, 10, 1, false);
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
    std::cout << "begining dot adjustment, THIS PROCESS IS SLOW" << std::endl;
    cv::Mat adjusted = dots(path, map, path, seeds, 6.0, false);
    std::cout << "finalized dot placement for image" << std::endl;

    // step 6: place circles!
    cv::Mat rendered = placeDots(path, adjusted, path, image, 20, true);
    std::cout << "sized dots" << std::endl;
    save(rendered, path, "-rendered");

    return rendered;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Must pass in image to run hedcut algorithm on."
                  << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            caboodle(argv[i], image, true);
        }
    }
}
