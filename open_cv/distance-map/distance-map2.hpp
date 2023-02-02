#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/type2str.hpp"
#include "../boiler-plate/read-save.hpp"
#include "sampled-transform-2.hpp"
#include "sampled-transform-seeds.hpp"

struct distMap
{
    cv::Mat distances;
    cv::Mat priorityBuffer;
};

distMap distanceMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos, cv::Mat imgIsos, bool saving);