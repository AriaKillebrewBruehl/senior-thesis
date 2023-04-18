#include "close-gaps.hpp"

cv::Mat closeGaps(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    // convert image to binary
    cv::Mat thresh;
    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
    cv::threshold(image, thresh, 127, 255, cv::THRESH_BINARY);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 3));
    cv::Mat src = image;
    cv::Mat closed;
    for (int i = 0; i < 1; i++) {
        cv::morphologyEx(src, closed, cv::MORPH_CLOSE, element);
        src = closed;
    }

    cv::Mat element2 =
        cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 1));
    src = image;
    for (int i = 0; i < 1; i++) {
        cv::morphologyEx(src, closed, cv::MORPH_CLOSE, element2);
        src = closed;
    }

    if (saving) {
        save(src, path, "-closed");
    }

    return src;
}