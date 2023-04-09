#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../dots/dots.hpp"
#include "../dots/place-dots.hpp"
#include "../dots/place-seeds.hpp"

int lane_size = 10;
cv::Mat image;
cv::Mat offset;
cv::Mat details;
cv::Mat seeds;
cv::Mat adjusted_dots;
cv::Mat stipples;

void compare_gray(std::string path, cv::Mat img);
#include "read-save.hpp"