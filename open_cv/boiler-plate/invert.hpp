#pragma once

#include <opencv2/opencv.hpp>
#include <string>

#include "read-save.hpp"
#include "type2str.hpp"

// Returns an inverted image if the input image has a black background
//
// parameters:
//
// arr - the input image
cv::Mat invert(cv::Mat arr);