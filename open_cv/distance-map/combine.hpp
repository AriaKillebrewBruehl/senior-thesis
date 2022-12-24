#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/type2str.hpp"
#include "../boiler-plate/read-save.hpp"

cv::Mat combine(std::string pathEdge, cv::Mat imgEdge, std::string pathIsos, cv::Mat imgIsos, bool saving);
