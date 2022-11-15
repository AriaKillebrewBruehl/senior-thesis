#pragma once
#include "distance-map.hpp"
#include "offset-map.hpp"
#include "combine.hpp"

cv::Mat fullMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos, cv::Mat imgIsos, bool saving);