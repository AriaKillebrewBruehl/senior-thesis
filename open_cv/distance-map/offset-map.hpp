#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "distance-map2.hpp"

cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, bool saving);