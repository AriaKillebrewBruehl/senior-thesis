#include <iostream>

#include "../boiler-plate/read-save.hpp"

/*
cv::Mat placeDots(std::string path, cv::Mat offsetMap, int d, bool saving)
Given an offset map with sections labeled NOT a
visual map or a map with lines labeled, randomly places seed pixels in black on
a white image.
*/
cv::Mat placeSeeds(std::string path, cv::Mat offsetMap, int d, bool saving);