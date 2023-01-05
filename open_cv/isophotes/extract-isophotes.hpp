#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "isophotes.hpp"
#include "../edge-detection/extract-edges.hpp"

cv::Mat extractIsophotes(std::string path, cv::Mat img, int edgethresh, int isothresh, bool saving);