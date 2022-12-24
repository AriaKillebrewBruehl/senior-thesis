#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"

cv::Mat skeleton(std::string path, cv::Mat img, bool saving);