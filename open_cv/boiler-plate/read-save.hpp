#pragma once

#include <opencv2/opencv.hpp>
#include <string>

#include "type2str.hpp"
// Returns a cv::Mat
//
// parameters:
//
// path - the file path to a .png
// (can be "" if img is not empty)
//
// img - the input image
// (can be empty if path is not "")
cv::Mat read(std::string path, cv::Mat img);

// Saves an image to the file system
//
// parameters:
// image - the image to be saved
//
// path - the file path to the save location
//
// suffix - the tag to add to path
void save(cv::Mat image, std::string path, std::string suffix);