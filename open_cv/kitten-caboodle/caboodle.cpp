#include "caboodle.hpp"

cv::Mat caboodle(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());

    // step 1: extract the edges of the image
    cv::Mat edges = extractEdges(path, image, 300, false);
    if (edges.type() != 0) {
        edges.convertTo(edges, 0);
    }
    save(edges, path, "-edges");
    std::cout << "extracted edges from image" << std::endl;
    int c = 0;
    while (!c) {
        std::cout << "continue? 0/1" << std::endl;
        system("pause");
        std::cin >> c;
    }
    // step 2: extract the isophotes of the image
    cv::Mat isophotes = extractIsophotes(path, image, 10, 5, false);
    if (isophotes.type() != 0) {
        isophotes.convertTo(isophotes, 0);
    }
    save(isophotes, path, "-isophotes");
    std::cout << "extracted isophotes from image" << std::endl;
    c = 0;
    while (!c) {
        std::cout << "continue? 0/1" << std::endl;
        system("pause");
        std::cin >> c;
    }
    // step 3: offset map
    cv::Mat map = fullMap(path, edges, path, isophotes, false);
    save(map, path, "-offsetmap");
    std::cout << "extracted offset map from image" << std::endl;

    c = 0;
    while (!c) {
        std::cout << "continue? 0/1" << std::endl;
        system("pause");
        std::cin >> c;
    }
    // step 4: generate final dot placement
    cv::Mat adjusted = dots(path, map, false);
    save(adjusted, path, "-adjusted");
    std::cout << "finalized dot placement for image" << std::endl;

    // step 5: place circles!
    c = 0;
    while (!c) {
        std::cout << "continue? 0/1" << std::endl;
        system("pause");
        std::cin >> c;
    }
    cv::Mat rendered = placeDots(path, adjusted, path, image, true);
    std::cout << "sized dots" << std::endl;

    return map;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Must pass in image to caboodle." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            caboodle(argv[i], image, true);
        }
    }
}
