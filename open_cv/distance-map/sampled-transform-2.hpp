#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/read-save.hpp"

// indicator function for membership in a set of seed pixels
int32_t func(int32_t x);

cv::Mat OneD(cv::Mat arr, std::function<int32_t(cv::Mat, int32_t)> f);

cv::Mat TwoD(cv::Mat arr, std::function<int32_t(cv::Mat, int32_t)> f);

cv::Mat sample(cv::Mat img, std::string path, bool saving);