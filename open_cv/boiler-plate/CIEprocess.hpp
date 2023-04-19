#pragma once

#include <opencv2/opencv.hpp>

#include "read-save.hpp"
#include "type2str.hpp"

// Returns a posterized grayscale version of the input image
//
// parameters:
//
// img - the input image
cv::Mat processColors(cv::Mat img, int bins);