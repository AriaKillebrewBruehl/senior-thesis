#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <opencv2/opencv.hpp>
#include "../edge-detection/type2str.cpp"
#include "save.cpp"

cv::Mat getIsophotes(std::string path, cv::Mat img, bool saving);