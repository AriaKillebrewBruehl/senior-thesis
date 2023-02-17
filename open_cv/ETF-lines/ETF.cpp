#include "ETF.hpp"

cv::Mat ETF(std::string path, cv::Mat img, bool saving) {
    cv ::Mat image;
    image = read(path, img);
    assert(!image.empty());
    if (image.type() != 0) {
        if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2GRAY);
        } else if (image.channels() == 3) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }
        image.convertTo(image, 0);
    }
}