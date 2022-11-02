#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "read-save.hpp"
#include "jfa.hpp"

cv::Mat distanceMap(std::string path, cv::Mat img, bool saving);