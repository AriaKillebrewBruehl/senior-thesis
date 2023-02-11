#include "place-dots.hpp"

cv::Mat placeDots(std::string path, cv::Mat img, bool saving) {
    cv::Mat image = read(path, img);
    assert(!image.empty());

    if (image.type() != 4) {
        if (image.channels() != 1) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }
        image.convertTo(image, 4);
    }
    assert(image.type() == 4);
    return image;
}