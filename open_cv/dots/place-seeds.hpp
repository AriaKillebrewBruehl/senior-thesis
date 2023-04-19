#include <iostream>

#include "../boiler-plate/read-save.hpp"

// Returns a image with a random sampling of seed pixels that avoid offset lines
//
// parameters:
//
// path - the file path to the offset map
// (can be "" if offsetMap is not empty)
//
// offsetMap - the offsetMap as a cv::Mat
// (can be empty if path is not "")
//
// d - the approximate distance between seed pixels
//
// saving - saves the seeds to the file system
cv::Mat placeSeeds(std::string path, cv::Mat offsetMap, int d, bool saving);

// Returns a image with a random sampling of seed pixels that avoid offset lines
// and consider distance from offset lines in seed placement
//
// parameters:
//
// pathOffset - the file path to the offset map
// (can be "" if offsetMap is not empty)
//
// offsetMap - the offsetMap as a cv::Mat
// (can be empty if pathOffset is not "")
//
// pathDists - the file path to the distance map
// (can be "" if dists is not empty)
//
// dists - the distance map as a cv::Mat
// (can be empty if pathDists is not "")
//
// d - the approximate distance between seed pixels
//
// saving - saves the seeds to the file system
cv::Mat placeSeedsAdjusted(std::string pathOffset, cv::Mat offsetMap,
                           std::string pathDists, cv::Mat dists, int d,
                           bool saving);