#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "canny.hpp"
#include "DoG.hpp"
#include "threshold-edges.hpp"
#include "type2str.hpp"
#include "read-save.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, bool saving);

