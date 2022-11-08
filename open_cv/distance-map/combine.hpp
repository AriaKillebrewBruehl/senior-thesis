#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "type2str.hpp"
#include "read-save.hpp"

cv::Mat combine(std::string pathEdge, cv::Mat imgEdge, std::string pathIsos, cv::Mat imgIsos, bool saving);
