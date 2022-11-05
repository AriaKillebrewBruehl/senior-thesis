#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "read-save.hpp"

cv::Mat cannyFilter(std::string path, cv::Mat img, bool saving);