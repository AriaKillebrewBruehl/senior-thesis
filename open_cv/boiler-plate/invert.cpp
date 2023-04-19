#include "invert.hpp"

cv::Mat invert(cv::Mat arr) {
    assert(!arr.empty());
    if (arr.type() != 0) {
        if (arr.channels() != 1) {
            cv::cvtColor(arr, arr, cv::COLOR_RGB2GRAY);
        }
        arr.convertTo(arr, 0);
    }

    if (cv::countNonZero(arr) == (arr.rows * arr.cols)) {
        // image is fully white, so just return it
        return arr;
    } else if (cv::countNonZero(arr) == 0) {
        // image is fully black, so invert and return
        cv::bitwise_not(arr, arr);
        return arr;
    }

    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =
        cv::connectedComponentsWithStats(arr, labels, stats, centroids);

    int col = stats.at<int>(cv::CC_STAT_LEFT);
    int row = stats.at<int>(cv::CC_STAT_TOP);
    // background is black so invert
    if (arr.at<uchar>(row, col) == 0) {
        cv::bitwise_not(arr, arr);
    }

    return arr;
}
