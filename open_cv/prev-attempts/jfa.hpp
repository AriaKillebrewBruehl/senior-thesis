#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "read-save.hpp"

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
 
        if (hash1 != hash2) {
            return hash1 ^ hash2;             
        }
        // If hash1 == hash2, their XOR is zero.
          return hash1;
    }
};

using pixel_type = std::pair<int, int>;
using seed_map = std::unordered_map<pixel_type, pixel_type, hash_pair>; 

const pixel_type undef(-1, -1);

seed_map get_seeds(cv::Mat img);
seed_map get_seeds2(cv::Mat img);
void resz(cv::Mat& image);
cv::Mat jmp_flood(std::string path, cv::Mat img, bool saving);
cv::Mat jmp_flood2(std::string path, cv::Mat img, bool saving);
seed_map jmp_flood_seeds(std::string path, cv::Mat img);