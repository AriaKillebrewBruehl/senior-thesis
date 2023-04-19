#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <string>
#include <unordered_map>

#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"

// Returns the morphological skeleton of an input image
// parameters:
//
// path - the file path to the input image
// (can be "" if img is not empty)
//
// img - the input image as a cv::Mat
// (can be empty if path is not "")
//
// saving - saves the skeleton to the file system
cv::Mat skeleton(std::string path, cv::Mat img, bool saving);