#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <string>
#include <unordered_map>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"

typedef std::pair<uchar, int> color_freq;

struct comp {
    constexpr bool operator()(std::pair<int, int> const& a,
                              std::pair<int, int> const& b) const noexcept {
        return a.first < b.first;
    }
};

cv::Mat posterize(std::string path, cv::Mat img,
                  std::unordered_map<uchar, int>* colors, int bins,
                  bool saving);

cv::Mat getIsophotes(std::string path, cv::Mat img, int thresh, bool saving);
