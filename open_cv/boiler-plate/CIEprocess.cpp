#include "CIEprocess.hpp"

inline uchar reduceVal(const uchar val, int bins) {
    return uchar(val / bins) * bins;
    // if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    // return 255;
}

// return a grayscale version of the image with only the L component
cv::Mat processColors(cv::Mat img, int bins) {
    cv::Mat gs = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            uchar L = reduceVal(img.at<cv::Vec3b>(i, j)[0], bins);
            gs.at<uchar>(i, j) = (L);
        }
    }
    return gs;
}