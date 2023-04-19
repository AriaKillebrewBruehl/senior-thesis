#include <math.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../boiler-plate/invert.hpp"
#include "../boiler-plate/read-save.hpp"

// Returns a matrix where initial seed pixels are marked
// parameters:
//
// img - initial seeds
cv::Mat3i get_seeds(cv::Mat img);
// Returns a matrix where pixel value is distance from closest seed pixel
// parameters:
//
// img - output of distance transform
cv::Mat1i get_distance(cv::Mat3i img);
// Returns the sampled distance transform of a single dimensional array
// parameters -
//
// arr - a single dimensional array
//
// f - a function f: G -> R where G is a single dimensional array or grid
cv::Mat3i OneD(cv::Mat3i arr, std::function<int32_t(cv::Mat, int32_t)> f);
// Returns the sampled distance transform of a two dimensional array
// parameters:
//
// arr - the input multidemensional array
//
// f - a function f: G -> R where G is a single dimensional array or grid
cv::Mat3i TwoD(cv::Mat3i arr, std::function<int32_t(cv::Mat, int32_t)> f);
// Returns the sampled distance transform of an input image
// parameters:
//
// img - the input image as a cv::Mat
// (can be empty if path is not "")
//
// path - the file path to the input image
// (can be "" if img is not empty)
//
// saving - saves the final distance map
//
// seeds - if true records the nearest seed pixel to each non-seed pixel
cv::Mat sample_seeds(cv::Mat img, std::string path, bool saving, bool seeds);