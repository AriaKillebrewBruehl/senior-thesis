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

    int i = -1;
    int j = -1;
    bool not_found = true;
    while (i < labels.rows && not_found) {
        i++;
        while (j < labels.cols && not_found) {
            j++;
            if (labels.at<int>(i, j) == 0) {
                std::cout << "hi" << std::endl;
                not_found = false;
            }
        }
    }

    // background is black so invert
    if (arr.at<int>(i, j) == 0) {
        cv::bitwise_not(arr, arr);
    }

    return arr;
}
