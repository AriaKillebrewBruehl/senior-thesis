#include <iostream>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "../boiler-plate/read-save.hpp"

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

struct sampled_pair {
    cv::Mat sampled;
    cv::Mat seeds;
};

// indicator function for membership in a set of seed pixels
int32_t func(int32_t x);
cv::Mat OneD(cv::Mat arr, std::function<int32_t(cv::Mat, int32_t)> f);
cv::Mat TwoD(cv::Mat arr, std::function<int32_t(cv::Mat, int32_t)> f);
cv::Mat sample(cv::Mat img, std::string path, bool saving);