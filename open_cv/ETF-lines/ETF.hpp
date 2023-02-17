#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/invert.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"

int ws(cv::Point2d x, cv::Point2d y, int r);
float g(cv::Point);
float wm(cv::Point2d x, cv::Point2d y, int u);
int wd(cv::Mat T, cv::Point2d x, cv::Point2d y);
int phi(cv::Mat T, cv::Point2d x, cv::Point2d y);
cv::Mat ETFFilter(cv::Mat Tcur, int r, int u, int k);
cv::Mat ETF(std::string path, cv::Mat img, bool saving);