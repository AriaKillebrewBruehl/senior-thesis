#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"

cv::Mat cannyFilter(std::string path, cv::Mat img, bool saving);