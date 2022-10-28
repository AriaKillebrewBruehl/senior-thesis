#pragma once 

#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>

#include "DoG.hpp"
#include "threshold-edges.hpp"
#include "type2str.cpp"

cv::Mat extractEdges(std::string path, cv::Mat img, bool saving);

