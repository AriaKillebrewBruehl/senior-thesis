#include "dots.hpp"

cv::Mat dots(std::string path, cv::Mat img, bool saving) {
    cv::Mat offsets = read(path, img);
    assert(!offsets.empty());

    if (offsets.type() != 4) {
        if (offsets.channels() != 1) {
            cv::cvtColor(offsets, offsets, cv::COLOR_RGB2GRAY);
        }
        offsets.convertTo(offsets, 4);
    }

    // step 0: generate seeds
    cv::Mat seeds = placeSeeds("", offsets, 6, true);
    std::cout << "placed seeds" << std::endl;

    // step 1: adjust seeds
    int t1 = 10;
    int t2 = 30;
    while (t1 || t2) {
        // std::cout << "current seeds: " << std::endl;
        // std::cout << seeds << std::endl;
        // // step 1: get distance map
        cv::Mat3i dists = sample_seeds(seeds, "", false, true);
        // std::cout << "current distance map:" << std::endl;
        // for (int i = 0; i < dists.rows; i++) {
        //     for (int j = 0; j < dists.cols; j++) {
        //         if (dists.at<cv::Vec3i>(i, j)[0] == 0) {
        //             std::cout << "(X, X)  ";
        //         } else {
        //             std::cout << "(" << dists.at<cv::Vec3i>(i, j)[1] << ", "
        //                       << dists.at<cv::Vec3i>(i, j)[2] << ")  ";
        //         }
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << dists << std::endl;
        // step 2: adjust seeds
        if (t1) {
            // no offset lane constraints
            seeds = adjust("", offsets, "", dists, false, false);
            t1--;
        } else {
            // alternate offset lane constraints
            seeds = adjust("", offsets, "", dists, false, t2 % 2);
            t2--;
        }
        // int c = 0;
        // while (c != 1) {
        //     std::cout << "continue? 1/0" << std::endl;
        //     std::cin >> c;
        // }
    }
    std::cout << "adjusted seeds" << std::endl;

    if (saving) {
        save(seeds, path, "-adjusted");
    }
    return seeds;
}