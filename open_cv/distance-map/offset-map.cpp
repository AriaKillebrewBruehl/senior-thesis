#include "offset-map.hpp"

cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists,bool saving) {
    // read images and resize
    cv::Mat dists;
    dists = read(pathDists, imgDists);
    try {
        if (dists.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in offsetMap." << std::endl;
        return dists;
    }
    if (dists.type() != 0) {
        std::cout << "ERROR: Input image must be single chanel in offsetMap." << std::endl;
        return dists;
    }
    
    // for (int i = 0; i < dists.rows; i++) {
    //     for (int j = 0; j < dists.cols; j++) { 
    //         std::cout << int(dists.at<uchar>(i,j)) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    cv::Mat visual = cv::Mat::zeros(dists.rows, dists.cols, CV_8UC1);
    cv::Mat offsetMap = cv::Mat::zeros(dists.rows, dists.cols, CV_8UC1);
    float l = 6.0;
    int w = 1;
    int id = 0;
    for (int i = 0; i < offsetMap.rows; i++) {
        for (int j = 0; j < offsetMap.cols; j++) { 
            float d = float(dists.at<uchar>(i, j));
            // std::cout << d << " " << ceil(d / l) << " " << ceil(d / l) * l - d << std::endl;
            float delta = ceil(d / l) * l - d;
            // std::cout << delta << " "; 
            if (delta <= w) {
                // std::cout << "(" << j << ", " << i << "), d: " << d << " delta: " << delta << std::endl; 
                int id = ceil(d / l);
                offsetMap.at<uchar>(i, j) = uchar(id);
                visual.at<uchar>(i, j) = uchar(255);
            } else {
                offsetMap.at<uchar>(i, j) = uchar(0);
                visual.at<uchar>(i, j) = uchar(0);
            }

            // std::cout << "(" << i << ", " << j << ")" << std::endl;
            // float R_min = ((l - w) * id) - (w / 2);
            // float R_max = ((l - w) * id) + (w / 2);
            // float delta = float(dists.at<uchar>(i, j)) * float(priorities.at<uchar>(i, j));
            // std::cout << "R_min: " << R_min << " R_max: " << R_max << " delta: " << delta << std::endl;

            // if (R_min <= delta && delta <= R_max) {
            //     std::cout << "hi" << std::endl;
            //     id++;
            //     offsetMap.at<uchar>(i, j) = uchar(id);
            //     visual.at<uchar>(i, j) = uchar(255);
            // } else {
            //     offsetMap.at<uchar>(i, j) = uchar(0);
            //     visual.at<uchar>(i, j) = uchar(0);
            // }
        }
        // std::cout << std::endl;
    }

    if (saving) {
        save(visual, pathDists, "-o-map");
    }
    return offsetMap;
}