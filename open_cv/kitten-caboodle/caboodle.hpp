#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "../distance-map/distance-map2.hpp"
#include "../edge-detection/extract-edges.hpp"
#include "../edge-detection/gray-scale.hpp"
#include "../isophotes/extract-isophotes.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"

cv::Mat caboodle(std::string path, cv::Mat img, bool saving);