#include <math.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "distance-map.hpp"

// parameters:
// pathDists - the file path to the distance map
// (can be "" if imgDists is not empty)
//
// imgDists - the distance map as a cv::Mat
// (can be empty if pathDists is not "")
//
// l - the integer distance between offset lines
//
// saving - if true saves the generated offset map to the file system
//
// sections - if true returns an offset map with
// black offset lines and sections of color corresponding to the section id,
// if false, returns an offset map with a white background and offset
// lines of color corrsponding to the line's id
cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, float l, bool saving,
                  bool sections);