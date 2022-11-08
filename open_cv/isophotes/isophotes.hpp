#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <opencv2/opencv.hpp>
#include "type2str.hpp"
#include "read-save.hpp"

cv::Mat getIsophotes(std::string path, cv::Mat img, bool saving);