#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <string>
#include <unordered_map>

#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "../edge-detection/extract-edges.hpp"
#include "isophotes.hpp"

cv::Mat extractIsophotes(std::string path, cv::Mat img, int edgethresh,
                         int isothresh, bool saving);