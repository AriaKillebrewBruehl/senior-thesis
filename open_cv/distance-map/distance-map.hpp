#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "type2str.hpp"
#include "read-save.hpp"
#include "jfa.hpp"

struct distMap {
    cv::Mat distances;
    cv::Mat priorityBuffer;
};

distMap distanceMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos, cv::Mat imgIsos, bool saving);