#include "place-seeds.hpp"

cv::Mat placeSeeds(std::string path, cv::Mat offsetMap, int d, bool saving) {
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
    cv::Mat seeds = cv::Mat(image.rows, image.cols, CV_8UC1, cv::Scalar(255));
    srand(time(0));
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            // feature lines are black
            if (image.at<int32_t>(i, j) != 0) {
                double r = ((double)rand()) / RAND_MAX;
                if (r <= 1 / double(d * d)) {
                    seeds.at<uchar>(i, j) = uchar(0);
                }
            }
        }
    }

    if (saving) {
        save(seeds, path, "-seeds");
    }

    return seeds;
}