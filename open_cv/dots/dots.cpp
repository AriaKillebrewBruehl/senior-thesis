#include "dots.hpp"

cv::Mat dots(std::string path, cv::Mat img, bool saving) {
    cv::Mat image = read(path, img);
    assert(!image.empty());

    // TODO type checking

    // step 0: generate seeds
    cv::Mat seeds = placeDots("", image, 6, false);
}