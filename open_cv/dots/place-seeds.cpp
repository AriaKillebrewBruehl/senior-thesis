#include "place-seeds.hpp"

cv::Mat placeSeeds(std::string pathOffset, cv::Mat offsetMap,
                   std::string pathDetails, cv::Mat imgDetails, int d,
                   bool saving) {
    cv::Mat offset;
    offset = read(pathOffset, offsetMap);

    assert(!offset.empty());
    if (offset.type() != CV_8UC1) {
        if (offset.channels() == 3) {
            cv::cvtColor(offset, offset, cv::COLOR_RGB2GRAY);
        } else if (offset.channels() == 4) {
            cv::cvtColor(offset, offset, cv::COLOR_RGBA2GRAY);
        }
        offset.convertTo(offset, CV_8UC1);
    }

    cv::Mat details;
    details = read(pathDetails, imgDetails);

    assert(!details.empty());
    if (details.type() != CV_8UC1) {
        if (details.channels() == 3) {
            cv::cvtColor(details, details, cv::COLOR_RGB2GRAY);
        } else if (details.channels() == 4) {
            cv::cvtColor(details, details, cv::COLOR_RGBA2GRAY);
        }
        details.convertTo(details, CV_8UC1);
    }

    // blank map for seed pixels
    cv::Mat seeds = cv::Mat(offset.size(), CV_8UC1, cv::Scalar(255));
    srand(time(0));
    for (int i = 0; i < seeds.rows; i++) {
        for (int j = 0; j < seeds.cols; j++) {
            // feature lines are black
            if (offset.at<uchar>(i, j) != 0) {
                double r = ((double)rand()) / RAND_MAX;
                int s = d;
                if (details.at<uchar>(i, j) == 255) {
                    // detailed sections are while
                    s /= 4;
                }
                if (r <= 1 / double(s * s)) {
                    seeds.at<uchar>(i, j) = uchar(0);
                }
            }
        }
    }

    if (saving) {
        save(seeds, pathOffset, "-seeds");
    }

    return seeds;
}

cv::Mat placeSeedsAdjusted(std::string pathOffset, cv::Mat offsetMap,
                           std::string pathDists, cv::Mat dists, int d,
                           bool saving) {
    cv::Mat offset;
    offset = read(pathOffset, offsetMap);

    assert(!offset.empty());
    if (offset.type() != CV_32SC1) {
        if (offset.channels() != 1) {
            cv::cvtColor(offset, offset, cv::COLOR_RGB2GRAY);
        }
        offset.convertTo(offset, CV_32SC1);
    }

    cv::Mat distancess;
    distancess = read(pathDists, dists);

    assert(!distancess.empty());
    std::cout << distancess.type() << std::endl;
    // if (offset.type() != CV_32SC1) {
    //     if (offset.channels() != 1) {
    //         cv::cvtColor(offset, offset, cv::COLOR_RGB2GRAY);
    //     }
    //     offset.convertTo(offset, CV_32SC1);
    // }

    // blank map for seed pixels
    cv::Mat seeds = cv::Mat(offset.rows, offset.cols, CV_8UC1, cv::Scalar(255));
    srand(time(0));
    for (int i = 0; i < offset.rows; i++) {
        for (int j = 0; j < offset.cols; j++) {
            // feature lines are black
            if (offset.at<int32_t>(i, j) != 0) {
                double r = ((double)rand()) / RAND_MAX;
                int s;
                int dist = distancess.at<int32_t>(i, j);
                if (dist < 5) {
                    s = d * 0.5;
                } else if (dist < 10) {
                    s = d * 0.75;
                } else {
                    s = d + (dist / 25);
                }
                if (r <= 1 / double(s * s)) {
                    seeds.at<uchar>(i, j) = uchar(0);
                }
            }
        }
    }

    if (saving) {
        save(seeds, pathOffset, "-seeds");
    }

    return seeds;
}