#include <iostream>

#include "../boiler-plate/read-save.hpp"
#include "../distance-map/sampled-transform-seeds.hpp"
#include "adjust-cells.hpp"
#include "place-dots.hpp"
#include "place-seeds.hpp"

// Returns the adjusted seeds
// parameters:
//
// pathOffset - the file path to the offset map with sections
// (can be "" if img_offset is not empty)
//
// pathSeeds - the file path to the seeds
// (can be "" is img_seeds is not empty)
//
// pathSeeds - the offset map with sections as a cv::Mat
// (can be empty if path_offset is not "")
//
// imgSeeds - the seeds as a cv::Mat
// (can be empty if path_seeds is not "")
//
// d - the approximate distance between seeds
//
// saving - save the adjusted seeds
cv::Mat dots(std::string pathOffset, cv::Mat imgOffset, std::string pathSeeds,
             cv::Mat imgSeeds, int d, bool saving);