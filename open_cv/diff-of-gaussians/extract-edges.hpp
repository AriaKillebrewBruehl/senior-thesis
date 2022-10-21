#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>

#include "DoG.hpp"

cv::Mat morph_open(std::string file_path, cv::Mat image);
cv::Mat skeleton(cv::Mat img);
cv::Mat DoG(std::string img);