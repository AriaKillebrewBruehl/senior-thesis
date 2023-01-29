#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/read-save.hpp"

// indicator function for membership in a set of seed pixels
cv::Mat3i get_seeds(cv::Mat img);
// extract just distance matrix from distance and seed matrix
cv::Mat1i get_distance(cv::Mat3i img);
// sampled transformation in one dimension
cv::Mat3i OneD(cv::Mat3i arr, std::function<int32_t(cv::Mat, int32_t)> f, int32_t n);
// sampled transformation in two dimensions 
cv::Mat3i TwoD(cv::Mat3i arr, std::function<int32_t(cv::Mat, int32_t)> f);
// sampled distance transformation with seed pixels
cv::Mat sample_seeds(cv::Mat img, std::string path, bool saving, bool seeds);