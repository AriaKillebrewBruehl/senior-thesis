#include "place-dots.hpp"

cv::Mat placeDots(std::string pathSeeds, cv::Mat imgSeeds, std::string pathOrig,
                  cv::Mat imgOrig, int s_max, bool saving) {
    cv::Mat seeds = read(pathSeeds, imgSeeds);
    assert(!seeds.empty());

    if (seeds.type() != 0) {
        if (seeds.channels() != 1) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGB2GRAY);
        }
        seeds.convertTo(seeds, 0);
    }
    assert(seeds.type() == 0);

    cv::Mat image = read(pathOrig, imgOrig);
    assert(!image.empty());

    if (image.type() != 0) {
        if (image.channels() != 1) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }
        image.convertTo(image, 0);
    }
    assert(image.type() == 0);

    int scale = 6;
    cv::Mat rendered = cv::Mat(image.rows * scale, image.cols * scale, CV_8UC1,
                               cv::Scalar(255));
    float gamma = 1.2;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (seeds.at<uchar>(i, j) == 0) {
                float t = float(image.at<uchar>(i, j)) / 255.0;
                float s = s_max * std::pow((1 - t), gamma);
                if (s >= 1.0) {
                    cv::circle(rendered, cv::Point2d(j * scale, i * scale),
                               int(s), cv::Scalar(0), -1);
                }
            }
        }
    }

    if (saving) {
        save(rendered, pathSeeds, "-rendered");
    }

    return rendered;
}