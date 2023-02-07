#include "offset-map.hpp"

cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, bool saving) {
    // read images and resize
    cv::Mat dists;
    dists = read(pathDists, imgDists);
    assert(!dists.empty());
    if (dists.type() != 0) {
        if (dists.channels() != 1) {
            cv::cvtColor(dists, dists, cv::COLOR_RGB2GRAY);
        }
        dists.convertTo(dists, 0);
    }

    cv::Mat visual = cv::Mat(dists.rows, dists.cols, CV_8UC1, cv::Scalar(255));
    cv::Mat offsetMap =
        cv::Mat(dists.rows, dists.cols, CV_8UC1, cv::Scalar(255));
    float l = 6.0;
    int w = 1;
    int id = 0;
    for (int i = 0; i < offsetMap.rows; i++) {
        for (int j = 0; j < offsetMap.cols; j++) {
            float d = float(dists.at<uchar>(i, j));
            float delta = ceil(d / l) * l - d;
            if (delta <= w) {
                int id = ceil(d / l);
                offsetMap.at<uchar>(i, j) = uchar(id);
                visual.at<uchar>(i, j) = uchar(0);
            }
        }
    }

    if (saving) {
        save(visual, pathDists, "-o-map-visual");
    }
    return offsetMap;
}