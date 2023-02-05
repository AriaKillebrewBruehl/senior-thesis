#include <iostream>

#include "../boiler-plate/read-save.hpp"
#include "../distance-map/sampled-transform-seeds.hpp"
#include "adjust-cells.hpp"
#include "place-dots.hpp"

// input image is offset map
cv::Mat dots(std::string path, cv::Mat img, bool saving);