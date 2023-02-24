#include <iostream>
#include <opencv2/opencv.hpp>

#include "../boiler-plate/read-save.hpp"

cv::Mat sobel_mag_angle(std::string path, cv::Mat img, bool saving);