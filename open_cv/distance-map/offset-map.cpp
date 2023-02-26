#include "offset-map.hpp"

cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, float l, bool saving,
                  bool sections) {
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

    cv::Mat map = cv::Mat(dists.rows, dists.cols, CV_32SC1, cv::Scalar(255));
    int w = 1;
    for (int i = 0; i < map.rows; i++) {
        for (int j = 0; j < map.cols; j++) {
            float d = float(dists.at<uchar>(i, j));
            float delta = ceil(d / l) * l - d;
            int id = ceil(d / l);
            // add offset line
            if (delta <= w) {
                map.at<int32_t>(i, j) = 0;
            } else {
                // if recording secitons give pixel id
                // otherwise keep white
                if (sections) {
                    map.at<int32_t>(i, j) = id + 1;
                }
            }
        }
    }

    if (saving) {
        save(map, pathDists, "-o-map-sections");
    }
    return map;
}