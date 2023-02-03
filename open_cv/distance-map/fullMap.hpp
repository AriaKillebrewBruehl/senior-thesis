#pragma once
#include "../boiler-plate/invert.hpp"
#include "combine.hpp"
#include "distance-map2.hpp"
#include "offset-map.hpp"

cv::Mat fullMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos,
                cv::Mat imgIsos, bool saving);