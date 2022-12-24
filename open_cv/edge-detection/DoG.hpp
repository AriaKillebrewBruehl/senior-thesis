#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/type2str.hpp"
#include "../boiler-plate/read-save.hpp"

cv::Mat DoG(std::string path, cv::Mat img, bool saving);