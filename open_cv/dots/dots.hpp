#include <iostream>

#include "../boiler-plate/read-save.hpp"
#include "adjust-cells.hpp"
#include "place-dots.hpp"

cv::Mat dots(std::string path_offset, cv::Mat img_offset,
             std::string path_seeds, cv::Mat img_seeds, bool saving);