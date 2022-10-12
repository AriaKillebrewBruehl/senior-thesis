#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>

int morph_open(std::string file_path, cv::Mat image);
cv::Mat DoG(std::string img);