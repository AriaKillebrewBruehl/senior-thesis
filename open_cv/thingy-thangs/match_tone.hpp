#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../dots/dots.hpp"
#include "../dots/place-dots.hpp"
#include "../dots/place-seeds.hpp"
#include "read-save.hpp"

cv::Mat image;

float compare_gray(std::string path, cv::Mat img);
