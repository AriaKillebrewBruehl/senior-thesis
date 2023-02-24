#include <iostream>
#include <opencv2/opencv.hpp>

#include "../boiler-plate/read-save.hpp"

struct g0 {
    cv::Mat magnitudes;
    cv::Mat directions;
};

g0 sobel_mag_angle(std::string path, cv::Mat img, bool saving);