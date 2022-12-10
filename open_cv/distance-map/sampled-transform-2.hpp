#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "read-save.hpp"

// indicator function for membership in a set of seed pixels
float32_t f(cv::Mat arr, float32_t p);

cv::Mat OneD(cv::Mat arr, std::function<float32_t(cv::Mat, float32_t)> f);

cv::Mat TwoD(cv::Mat arr, std::function<float32_t(cv::Mat, float32_t)> f);

cv::Mat sample(cv::Mat img, std::string path, bool saving);