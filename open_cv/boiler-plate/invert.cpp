#include "invert.hpp"

cv::Mat invert(cv::Mat arr) {
    assert(!arr.empty());
    if (arr.type() != 0) {
        if (arr.channels() != 1) {
            cv::cvtColor(arr, arr, cv::COLOR_RGB2GRAY);
        }
        arr.convertTo(arr, 0);
    }
    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =
        cv::connectedComponentsWithStats(arr, labels, stats, centroids);

    int x = stats.at<int>(cv::Point(0, 0));
    int y = stats.at<int>(cv::Point(1, 0));

    // background is black so invert
    if (arr.at<int>(y, x) == 0) {
        cv::bitwise_not(arr, arr);
    }

    return arr;
}
