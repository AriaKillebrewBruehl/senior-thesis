#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../dots/dots.hpp"
#include "../dots/place-dots.hpp"
#include "../dots/place-seeds.hpp"
#include "read-save.hpp"

int lane_size = 4;
cv::Mat image;
cv::Mat offset;
cv::Mat details;
cv::Mat seeds;
cv::Mat adjusted_dots;
int max_size;
cv::Mat stipples;

float compare_gray(std::string path, cv::Mat img);
