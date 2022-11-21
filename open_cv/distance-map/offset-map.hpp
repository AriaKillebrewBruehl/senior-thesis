#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "distance-map.hpp"

cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, std::string pathPriorities, cv::Mat imgPriorities, bool saving);