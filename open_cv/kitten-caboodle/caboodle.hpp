#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "../distance-map/fullMap.hpp"
#include "../dots/dots.hpp"
#include "../edge-detection/extract-edges.hpp"
#include "../edge-detection/gray-scale.hpp"
#include "../isophotes/extract-isophotes.hpp"

cv::Mat caboodle(std::string path, cv::Mat img, bool saving);