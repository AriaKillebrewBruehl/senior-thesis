#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <opencv2/opencv.hpp>
#include "skeleton.hpp"


cv::Mat threshold(std::string path, cv::Mat img, int threshold);
bool meetsThreshold(cv::Mat img, int threshold);