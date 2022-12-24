#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <opencv2/opencv.hpp>
#include "skeleton.hpp"
#include "../boiler-plate/read-save.hpp"

cv::Mat closeGaps(std::string path, cv::Mat img, bool saving);