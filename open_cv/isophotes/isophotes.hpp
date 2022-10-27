#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <opencv2/opencv.hpp>
#include "../diff-of-gaussians/type2str.cpp"

cv::Mat getIsophotes(std::string path, cv::Mat img);