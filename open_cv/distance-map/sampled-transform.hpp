#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "read-save.hpp"

// indicator function for membership in a set of seed pixels
int f(cv::Mat arr, int p);

cv::Mat DTOneDim(cv::Mat arr, std::function<int(int)> f);

cv::Mat test(cv::Mat arr, std::string path);

cv::Mat sample(cv::Mat img, std::string path, bool saving);