#pragma once

#include <iostream>
#include <string>
#include <random>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "skeleton.hpp"
#include "save.cpp"
#include "read.cpp"

cv::Mat threshold(std::string path, cv::Mat img, int threshold, bool saving);
bool meetsThreshold(cv::Mat img, int threshold);