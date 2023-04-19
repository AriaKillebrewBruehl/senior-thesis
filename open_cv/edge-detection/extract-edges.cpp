#include "extract-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, int thresh, bool saving) {
    // read images and resize
    cv::Mat image;
    image = read(path, img);
    cv::Mat correct;

    assert(!image.empty());

    if (image.type() != 16) {
        if (image.channels() == 1) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
        } else if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
        }
        image.convertTo(image, 16);
    }
    assert(image.type() == 16);

    cv::Mat edges = cannyFilter(path, image, false);

    // extract edges via threshold
    cv::Mat extracted = threshold(path, edges, thresh, false);
    cv::Mat inverted = invert(extracted);
    extracted.convertTo(extracted, CV_8UC1);
    cv::medianBlur(extracted, extracted, 5);

    inverted = invert(extracted);

    if (saving) {
        save(inverted, path, "-extracted-m6");
    }
    return inverted;
}
