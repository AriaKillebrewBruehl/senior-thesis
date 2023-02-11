#include <iostream>

#include "../boiler-plate/read-save.hpp"

cv::Mat placeDots(std::string pathSeeds, cv::Mat imgSeeds, std::string pathOrig,
                  cv::Mat imgOrig, bool saving);