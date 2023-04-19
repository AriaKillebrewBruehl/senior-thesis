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

// Returns a posterized version of the input image
// parameters:
//
// path - file path to input image
// (can be "" if img is not empty)
//
// img - the input image as a cv::Mat
// (can be empty if path is not "")
//
// bins - the number of bins for the image to be quantized with
//
// saving - saves the posterized image to the file system

cv::Mat posterize(std::string path, cv::Mat img, int bins, bool saving);

// Returns binary image with posterization
// parameters:
//
// path - file path to input image
// (can be "" if img is not empty)
//
// img - the input image as a cv::Mat
// (can be empty if path is not "")
//
// thresh - the fraction of posterized sections to be taken
//
// saving - saves the posterized image to the file system
cv::Mat getIsophotes(std::string path, cv::Mat img, int thresh, bool saving);
