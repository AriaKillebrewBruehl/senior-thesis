#pragma once

#include <opencv2/opencv.hpp>

#include "type2str.hpp"

cv::Mat processColors(cv::Mat img, std::unordered_map<uchar, int> *colors);