#include "place-dots.hpp"

cv::Mat placeDots(std::string pathSeeds, cv::Mat imgSeeds, std::string pathOrig,
                  cv::Mat imgOrig, bool saving) {
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

    // if (image.type() != 16) {
    //     if (image.channels() != 3) {
    //         std::cout << "Original image must be a 3 channel color image!";
    //         abort();
    //     }
    //     image.convertTo(image, 6);
    // }
    // assert(image.type() == 16);

    // step 1: scale up image
    int scale = 6;
    cv::Mat enlarged = cv::Mat(seeds.rows * scale, seeds.cols * scale, CV_8UC1,
                               cv::Scalar(255));

    for (int i = 0; i < seeds.rows; i++) {
        for (int j = 0; j < seeds.cols; j++) {
            if (seeds.at<uchar>(i, j) == uchar(0)) {
                enlarged.at<uchar>(i * scale, j * scale) = uchar(0);
            }
        }
    }

    // cv::Mat rendered =
    //     cv::Mat(enlarged.rows, enlarged.cols, CV_8UC1, cv::Scalar(255));
    // cv::Mat T = processColors(image, nullptr);
    // std::cout << T << std::endl;
    // float gamma = 1.2;
    // int s_max = 6;
    // for (int i = 0; i < enlarged.rows; i++) {
    //     for (int j = 0; j < enlarged.cols; j++) {
    //         if (enlarged.at<uchar>(i, j) == 0) {
    //             float s = s_max * std::pow((1 - T.at<uchar>(i, j)), gamma);
    //             std::cout << s << std::endl;
    //             if (s >= 1.5) {
    //                 cv::circle(rendered, cv::Point2d(j, i), int(s),
    //                            cv::Scalar(0), -1);
    //             }
    //         }
    //     }
    // }

    if (saving) {
        save(enlarged, pathSeeds, "-enlarged");
    }
    std::cout << "saved" << std::endl;

    return enlarged;
}