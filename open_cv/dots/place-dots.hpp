#include <iostream>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/invert.hpp"
#include "../boiler-plate/read-save.hpp"

cv::Mat placeDots(std::string pathSeeds, cv::Mat imgSeeds, std::string pathOrig,
                  cv::Mat imagOrig, int s_max, bool saving);

cv::Mat placeDotsNegativeSpace(std::string pathSeeds, cv::Mat imgSeeds,
                               std::string pathOrig, cv::Mat imgOrig,
                               std::string pathNegative, cv::Mat imgNegative,
                               std::string pathDetails, cv::Mat imgDetails,
                               int s_max, bool saving);