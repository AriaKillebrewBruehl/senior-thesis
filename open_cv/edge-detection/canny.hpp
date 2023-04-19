#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"

// Return the edges detected with the Canny filter
//
// parameters:
//
// path - the file path to the image
// (can be "" if img is not empty)
//
// img - the image as a cv::Mat
// (can be empty if path is not "")
//
// saving - saves the edges to the file system
cv::Mat cannyFilter(std::string path, cv::Mat img, bool saving);