#pragma once

#include <string>
#include <opencv2/opencv.hpp>

cv::Mat read(std::string path, cv::Mat img);
void save(cv::Mat image, std::string path, std::string suffix);