#include "caboodle.hpp"

cv::Mat caboodle(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());
    if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
        save(image, path, "-rgb");
    }
    int r = 0;
    std::cout << "stop after each step or run straight through? 0/1"
              << std::endl;
    std::cin >> r;

    // step 1: extract the edges of the image
    cv::Mat edges = extractEdges(path, image, 300, false);
    if (edges.type() != 0) {
        edges.convertTo(edges, 0);
    }
    save(edges, path, "-edges");
    std::cout << "extracted edges from image" << std::endl;
    int c = 0;
    if (r == 0) {
        std::cout << "continue/quit? 0/1" << std::endl;
        std::cin >> c;
        if (!c) {
            abort();
        }
    }

    // step 2: extract the isophotes of the image
    cv::Mat isophotes = extractIsophotes(path, image, 10, 5, false);
    if (isophotes.type() != 0) {
        isophotes.convertTo(isophotes, 0);
    }
    save(isophotes, path, "-isophotes");
    std::cout << "extracted isophotes from image" << std::endl;
    if (r == 0) {
        c = 0;
        std::cout << "continue/quit? 0/1" << std::endl;
        std::cin >> c;
        if (!c) {
            abort();
        }
    }
    // step 3: offset map
    cv::Mat map = fullMap(path, edges, path, isophotes, false);
    save(map, path, "-offsetmap");
    std::cout << "extracted offset map from image" << std::endl;

    if (r == 0) {
        c = 0;
        std::cout << "continue/quit? 0/1" << std::endl;
        std::cin >> c;
        if (!c) {
            abort();
        }
    }
    // step 4: generate final dot placement
    cv::Mat adjusted = dots(path, map, false);
    save(adjusted, path, "-adjusted");
    std::cout << "finalized dot placement for image" << std::endl;

    // step 5: place circles!
    if (r == 0) {
        c = 0;
        std::cout << "continue/quit? 0/1" << std::endl;
        std::cin >> c;
        if (!c) {
            abort();
        }
    }
    cv::Mat rendered = placeDots(path, adjusted, path, image, true);
    std::cout << "sized dots" << std::endl;

    cv::Mat bigEdges = cv::Mat(rendered.size(), CV_8UC1, cv::Scalar(255));
    cv::resize(edges, bigEdges, bigEdges.size(), 0, 0, cv::INTER_LINEAR);
    cv::threshold(bigEdges, bigEdges, 254, 255, cv::THRESH_BINARY);
    cv::Mat dotEdges = cv::Mat(rendered.size(), rendered.type());
    bitwise_and(rendered, bigEdges, dotEdges);
    save(dotEdges, path, "-combined-edges-2");

    cv::Mat bigIsophotes = cv::Mat(rendered.size(), CV_8UC1, cv::Scalar(255));
    cv::resize(isophotes, bigIsophotes, bigIsophotes.size(), 0, 0,
               cv::INTER_LINEAR);
    cv::threshold(bigIsophotes, bigIsophotes, 254, 255, cv::THRESH_BINARY);
    cv::Mat dotEdgesIsophotes = cv::Mat(rendered.size(), rendered.type());
    bitwise_and(dotEdges, bigIsophotes, dotEdgesIsophotes);
    save(dotEdgesIsophotes, path, "-combined-isophotes-2");

    return rendered;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Must pass in image to caboodle." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            int k = 3;
            int j = 2;
            int c = k / j;
            std::cout << c << std::endl;
            // cv::Mat a = read(argv[i], image);
            // cv::Mat b = read(argv[i + 1], image);
            // cv::Mat o = cv::Mat(a.size(), a.type());
            // cv::bitwise_and(a, b, o);
            // save(o, argv[i], "-anded");
            caboodle(argv[i], image, true);
        }
    }
}
