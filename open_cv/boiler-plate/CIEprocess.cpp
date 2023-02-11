#include "CIEprocess.hpp"

inline uchar reduceVal(const uchar val) {
    if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    return 255;
}

// return a grayscale version of the image with only the L component
// TODO: change this to std::transform?
cv::Mat processColors(cv::Mat img, std::unordered_map<uchar, int> *colors) {
    cv::Mat gs = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            uchar L = reduceVal(img.at<cv::Vec3b>(i, j)[0]);
            if (colors != nullptr) {
                (*colors)[L]++;
            }
            gs.at<uchar>(i, j) = (L);
        }
    }
    return gs;
}