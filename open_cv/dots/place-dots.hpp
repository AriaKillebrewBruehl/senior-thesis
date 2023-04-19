#include <iostream>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/invert.hpp"
#include "../boiler-plate/read-save.hpp"

// Returns a rendered stipple image
//
// parameters:
//
// pathSeeds - the file path to the adjusted seed pixels
// (can be "" if imgSeeds is not empty)
//
// imgSeeds - the adjusted seed pixels as a cv::Mat
// (can be empty if pathSeeds is not "")
//
// pathOrig - the file path to the original input image
// (can be "" if imgOrig is not empty)
//
// imgOrig - the original input image as a cv::Mat
// (can be empty if pathOrig is not "")
//
// s_max - the maximum stipple size
//
// saving - saves the stipple image to the file system
cv::Mat placeDots(std::string pathSeeds, cv::Mat imgSeeds, std::string pathOrig,
                  cv::Mat imagOrig, int s_max, bool saving);

// Returns a rendered stipple image with negative space
//
// parameters:
//
// pathSeeds - the file path to the adjusted seed pixels
// (can be "" if imgSeeds is not empty)
//
// imgSeeds - the adjusted seed pixels as a cv::Mat
// (can be empty if pathSeeds is not "")
//
// pathOrig - the file path to the original input image
// (can be "" if imgOrig is not empty)
//
// imgOrig - the original input image as a cv::Mat
// (can be empty if pathOrig is not "")
//
// // pathNegative - the file path to the negative space image
// (can be "" if imgNegative is not empty)
//
// imgNegative - the negative space image as a cv::Mat
// (can be empty if pathNegative is not "")
//
// s_max - the maximum stipple size
//
// saving - saves the stipple image to the file system
cv::Mat placeDotsNegativeSpace(std::string pathSeeds, cv::Mat imgSeeds,
                               std::string pathOrig, cv::Mat imgOrig,
                               std::string pathNegative, cv::Mat imgNegative,
                               std::string pathDetails, cv::Mat imgDetails,
                               int s_max, bool saving);