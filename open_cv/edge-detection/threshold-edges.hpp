#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <string>
#include <unordered_map>

#include "../boiler-plate/read-save.hpp"
#include "skeleton.hpp"

// Removes edges that do not meet the threshold length from the input image
//
// parameters:
//
// path - the file path to the input edges
// (can be "" if img is not empty)
//
// img - the input edges as a cv::Mat
// (can be empty if path is not "")
//
// threshold - the minimum edge length
//
// saving - saves the thresholded edges to the file system
cv::Mat threshold(std::string path, cv::Mat img, int threshold, bool saving);

// Determines if the number of white pixels in an image meets a threshold
//
// parameters:
//
// img - the input image as a cv::Mat
//
// threshold - the minimum number of white pixels in the input image
bool meetsThreshold(cv::Mat img, int threshold);