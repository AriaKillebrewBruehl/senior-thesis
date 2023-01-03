#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/type2str.hpp"
#include "../boiler-plate/read-save.hpp"

typedef std::pair<int, int> color_freq;

struct comp {
    constexpr bool operator()(
    std::pair<int, int> const& a,
    std::pair<int, int> const& b)
    const noexcept
    {
        // min heap
        return a.second < b.second;
    }
};

cv::Mat getIsophotes(std::string path, cv::Mat img, bool saving);
void processColors(cv::Mat& img);