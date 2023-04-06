#include "place-dots.hpp"

cv::Mat placeDots(std::string pathSeeds, cv::Mat imgSeeds, std::string pathOrig,
                  cv::Mat imgOrig, int s_max, bool saving) {
    cv::Mat seeds = read(pathSeeds, imgSeeds);
    assert(!seeds.empty());

    if (seeds.type() != 0) {
        if (seeds.channels() == 3) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGB2GRAY);
        } else if (seeds.channels() == 4) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGBA2GRAY);
        }
        seeds.convertTo(seeds, 0);
    }
    assert(seeds.type() == 0);

    cv::Mat image = read(pathOrig, imgOrig);
    assert(!image.empty());

    if (image.type() != 0) {
        if (image.channels() == 3) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        } else if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2GRAY);
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

cv::Mat placeDotsNegativeSpace(std::string pathSeeds, cv::Mat imgSeeds,
                               std::string pathOrig, cv::Mat imgOrig,
                               std::string pathNegative, cv::Mat imgNegative,
                               std::string pathDetails, cv::Mat imgDetails,
                               int s_max, bool saving) {
    cv::Mat seeds = read(pathSeeds, imgSeeds);
    assert(!seeds.empty());

    if (seeds.type() != 0) {
        if (seeds.channels() == 3) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGB2GRAY);
        } else if (seeds.channels() == 4) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGBA2GRAY);
        }
        seeds.convertTo(seeds, 0);
    }
    assert(seeds.type() == 0);

    cv::Mat image = read(pathOrig, imgOrig);
    assert(!image.empty());

    if (image.type() != 0) {
        if (image.channels() == 3) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        } else if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2GRAY);
        }
        image.convertTo(image, 0);
    }
    assert(image.type() == 0);

    cv::Mat negative_space = read(pathNegative, imgNegative);
    assert(!negative_space.empty());

    if (negative_space.type() != 0) {
        if (negative_space.channels() == 3) {
            cv::cvtColor(negative_space, negative_space, cv::COLOR_RGB2GRAY);
        } else if (negative_space.channels() == 4) {
            cv::cvtColor(negative_space, negative_space, cv::COLOR_RGBA2GRAY);
        }
        negative_space.convertTo(negative_space, 0);
    }
    assert(negative_space.type() == 0);
    int negativeZeros = cv::countNonZero(negative_space);
    if (negativeZeros == (negative_space.rows * negative_space.cols)) {
        // if all of negative_space is white then invert it
        cv::bitwise_not(negative_space, negative_space);
    }

    cv::Mat details = read(pathDetails, imgDetails);
    assert(!details.empty());

    if (details.type() != 0) {
        if (details.channels() == 3) {
            cv::cvtColor(details, details, cv::COLOR_RGB2GRAY);
        } else if (details.channels() == 4) {
            cv::cvtColor(details, details, cv::COLOR_RGBA2GRAY);
        }
        details.convertTo(details, 0);
    }

    assert(details.type() == 0);
    int detailZeros = cv::countNonZero(details);
    if (detailZeros == (details.rows * details.cols)) {
        // if all of details is white then invert it
        cv::bitwise_not(details, details);
    }

    int scale = 6;
    cv::Mat rendered = cv::Mat(image.rows * scale, image.cols * scale, CV_8UC1,
                               cv::Scalar(255));
    float gamma = 1.2;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            // white portions of negative space image will be avoided
            if (negative_space.at<uchar>(i, j) == 255) {
                continue;
            }
            if (seeds.at<uchar>(i, j) == 0) {
                float t = float(image.at<uchar>(i, j)) / 255.0;
                float s = s_max * std::pow((1 - t), gamma);
                if (details.at<uchar>(i, j) == 0) {
                    // make dots in less detailed area (black) bigger
                    s *= 2;
                } else {
                    s *= 1;
                }
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