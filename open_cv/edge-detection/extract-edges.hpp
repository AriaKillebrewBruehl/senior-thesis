#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "canny.hpp"
#include "DoG.hpp"
#include "threshold-edges.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, int thresh, bool saving);

