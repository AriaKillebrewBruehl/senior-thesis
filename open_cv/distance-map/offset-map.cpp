#include "offset-map.hpp"

cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, bool saving,
                  bool lines) {
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

    // for visual output only, not to be used in other sections of code
    cv::Mat visual = cv::Mat(dists.rows, dists.cols, CV_8UC1, cv::Scalar(255));
    // each offset line is tagged with an id on a white background
    cv::Mat offsetMapLines =
        cv::Mat(dists.rows, dists.cols, CV_32SC1, cv::Scalar(255));
    // each offset line is black and each section is tagged with an id
    cv::Mat offsetMapSections =
        cv::Mat(dists.rows, dists.cols, CV_32SC1, cv::Scalar(255));
    float l = 6.0;
    int w = 1;
    int id = 0;
    for (int i = 0; i < offsetMapLines.rows; i++) {
        for (int j = 0; j < offsetMapLines.cols; j++) {
            float d = float(dists.at<uchar>(i, j));
            float delta = ceil(d / l) * l - d;
            int id = ceil(d / l);
            // add offset line
            if (delta <= w) {
                visual.at<uchar>(i, j) = uchar(0);
                offsetMapLines.at<int32_t>(i, j) = id;
                offsetMapSections.at<int32_t>(i, j) = 0;
            } else {
                offsetMapSections.at<int32_t>(i, j) = id + 1;
            }
        }
    }

    if (saving) {
        save(offsetMapSections, pathDists, "-o-map-visual");
    }
    if (lines) {
        return offsetMapLines;
    } else {
        return offsetMapSections;
    }
}