#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>

#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "sampled-transform-seeds.hpp"

cv::Mat distanceMap(std::string pathEdges, cv::Mat imgEdges,
                    std::string pathIsos, cv::Mat imgIsos, bool saving);