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

// Returns a seed map with the centroids adjusted
// parameters:
//
// path_offset - the file path to the offset map with sections
// (can be "" if img_offset is not empty)
//
// path_seeds - the file path to the seeds
// (can be "" is img_seeds is not empty)
//
// img_offset - the offset map with sections as a cv::Mat
// (can be empty if path_offset is not "")
//
// img_seeds - the seeds as a cv::Mat
// (can be empty if path_seeds is not "")
//
// saving - save the adjusted seeds
//
// offset - offset line constraints will be considered
cv::Mat adjust(std::string path_offset, cv::Mat img_offset,
               std::string path_seeds, cv::Mat img_seeds, bool saving,
               bool offset);

// generate_map returns a map of type
//  std::unordered_map<pixel_type, std::vector<pixel_type>>
// where map[(x,y)] = [(x',y')] means that pixel (x',y') is the the Voronoi cell
// with pixel (x,y) as its seed
// parameters:
//
// image - distance map as a cv::Mat
seed_map generate_map(cv::Mat image);