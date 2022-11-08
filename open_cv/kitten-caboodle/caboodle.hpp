#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "../distance-map/distance-map.hpp"
#include "../edge-detection/extract-edges.hpp"
#include "../isophotes/extract-isophotes.hpp"
#include "read-save.hpp"
#include "type2str.hpp"

cv::Mat caboodle(std::string path, cv::Mat img, bool saving);