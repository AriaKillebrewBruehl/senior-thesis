#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/invert.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "canny.hpp"
#include "threshold-edges.hpp"

// Returns edges extracted from input image
// parameters:
//
// path - file path to input image
// (can be "" if img is not empty)
//
// img - the input image as a cv::Mat
// (can be empty if path is not "")
//
// saving - saves edges to file system
cv::Mat extractEdges(std::string path, cv::Mat img, int thresh, bool saving);
