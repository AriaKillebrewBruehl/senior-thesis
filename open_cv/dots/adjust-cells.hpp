#include <iostream>

#include "../boiler-plate/read-save.hpp"

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2> &p) const {
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
using seed_map =
    std::unordered_map<pixel_type, std::vector<pixel_type>, hash_pair>;

/*
cv::Mat adjust(std::string path_offset, cv::Mat img_offset,
               std::string path_seeds, cv::Mat img_seeds, bool saving,
               bool offset);

    Given an offset map that labels sections NOT a visual map or a map
    labeling lines and a seed map, return a seed map with the centroids
    adjusted. If offset is set offset line constraints will be considered.
*/
cv::Mat adjust(std::string path_offset, cv::Mat img_offset,
               std::string path_seeds, cv::Mat img_seeds, bool saving,
               bool offset);

/*
seed_map generate_map(cv::Mat image)
Given a seed map image return a map std::unordered_map<pixel_type,
std::vector<pixel_type>> where map[(x,y)] = [(x',y')] means that pixel
(x',y') is the the Voronoi cell with pixel (x,y) as its seed
*/
seed_map generate_map(cv::Mat image);