#include "invert.hpp"

cv::Mat invert(cv::Mat arr) {
    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =
        cv::connectedComponentsWithStats(arr, labels, stats, centroids);

    int i;
    int j;
    for (i = 0; i < labels.rows; i++) {
        for (j = 0; j < labels.cols; j++) {
            if (labels.at<int>(i, j) == 0) {
                break;
            }
        }
    }

    // background is black so invert
    if (arr.at<int>(i, j) == 0) {
        cv::bitwise_not(arr, arr);
    }

    return arr;
}
