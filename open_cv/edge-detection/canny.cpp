#include "canny.hpp"

cv::Mat cannyFilter(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());

    if (image.type() != 0) {
        if (image.channels() != 1) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }
        image.convertTo(image, 0);
    }

    cv::Mat dst, edges;

    int threshold = 40;
    const int ratio = 3;
    const int kernel_size = 3;

    dst.create(image.size(), image.type());

    float sigma = 0.33;

    double m = cv::mean(image)[0];

    // int lower = int(std::max(0.0, (1.0 - sigma) * m));
    // int upper = int(std::min(255.0, (1.0 + sigma) * m));
    int lower = 75;
    int upper = 125;
    // blur image to remove noise
    cv::blur(image, edges, cv::Size(3, 3));

    // apply Canny filter
    cv::Canny(edges, edges, lower, upper, kernel_size);

    cv::Mat element =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    cv::morphologyEx(edges, edges, cv::MORPH_DILATE, element, cv::Point(-1, -1),
                     2);
    cv::medianBlur(edges, edges, 3);
    // cv::Mat element2 =
    //     cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 1));
    // cv::morphologyEx(edges, edges, cv::MORPH_OPEN, element2, cv::Point(-1,
    // -1),
    //                  8);

    if (saving) {
        save(edges, path, "-canny-75-125-d2-m3");
    }
    return edges;
}
