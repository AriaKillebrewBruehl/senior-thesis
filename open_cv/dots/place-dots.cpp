#include "place-dots.hpp"

cv::Mat placeDots(std::string path, cv::Mat offsetMap, int d, bool saving) {
    cv::Mat image;
    image = read(path, offsetMap);

    assert(!image.empty());
    if (image.type() != CV_32SC1) {
        if (image.channels() != 1) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }
        image.convertTo(image, CV_32SC1);
    }

    // blank map for seed pixels
    cv::Mat seeds = cv::Mat::zeros(image.rows, image.cols, CV_32SC1);
    srand(time(0));
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            // feature lines are white
            if (image.at<int32_t>(i, j) != 255) {
                double r = ((double)rand()) / RAND_MAX;
                if (r <= 1 / double(d * d)) {
                    seeds.at<int32_t>(i, j) = 255;
                }
            }
        }
    }

    if (saving) {
        save(seeds, path, "-dots");
    }

    return seeds;
}