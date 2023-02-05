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

// Given an offset matrix and a seed matrix adjust seed pixels towards centers
// of offset lanes
cv::Mat adjust(std::string path_offset, cv::Mat img_offset,
               std::string path_seeds, cv::Mat img_seeds, bool saving,
               bool offset);

// Given a seed matrix generate a map[pixel_type]std::vector<pixel_type> for
// seed pixels and their neighbors
seed_map generate_map(cv::Mat image);