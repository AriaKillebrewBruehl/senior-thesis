#include <iostream>

#include "../boiler-plate/read-save.hpp"
#include "../distance-map/sampled-transform-seeds.hpp"
#include "adjust-cells.hpp"
#include "place-dots.hpp"
#include "place-seeds.hpp"

// Given an offset map return the adjusted seeds for the map.
// Make sure input is offset map with ids, NOT the visual map!
cv::Mat dots(std::string path, cv::Mat img, bool saving);