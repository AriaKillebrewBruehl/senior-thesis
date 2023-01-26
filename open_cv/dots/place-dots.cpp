#include "place-dots.hpp"

cv::Mat placeDots(std::string path, cv::Mat offsetMap, int d, bool saving) {
    cv::Mat image;
    image = read(path, offsetMap);

    // blank map for seed pixels
    cv::Mat seeds = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {

        }
    }


    if (saving) {
        save(image, path, "-dots");
    }

    return image;
}