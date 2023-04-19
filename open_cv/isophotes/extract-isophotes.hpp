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

// Returns the isophotes detected from an input image
// parameters:
//
// path - the file path to the input image
// (can be "" is img is not empty)
//
// img - the input image as a cv::Mat
// (can be empty if path is not "")
//
// edgethresh - the minimum length of the isophotes
//
// isothresh - the fraction of posterized sections taken
//
// saving - saves the isophotes to the file system
cv::Mat extractIsophotes(std::string path, cv::Mat img, int edgethresh,
                         int isothresh, int bins, bool saving);