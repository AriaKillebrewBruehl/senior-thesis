#include "adjust-cells.hpp"

seed_map generate_map(cv::Mat image) {
    assert(image.channels() == 3);
    seed_map map{};

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            pixel_type seed;
            seed.first = image.at<cv::Vec3i>(i, j)[1];
            seed.second = image.at<cv::Vec3i>(i, j)[2];

            pixel_type current;
            current.first = i;
            current.second = j;

            map[seed].push_back(current);
        }
    }
    return map;
}

cv::Mat adjust(std::string path_offset, cv::Mat img_offset,
               std::string path_seeds, cv::Mat img_seeds, bool saving) {
    cv::Mat offsets = read(path_offset, img_offset);
    cv::Mat seeds = read(path_seeds, img_seeds);

    // TODO type checking
    // given a distance map
    seed_map map = generate_map(seeds);

    for (auto const &pair : map) {
        pixel_type center = pair.first;
        int32_t x_sum;
        int32_t y_sum;
        int32_t w = 1;
        for (auto const &p : pair.second) {
            int32_t x = p.first;
            int32_t y = p.second;
            // if p is part of an offset line don't include it
            if (offsets.at<int32_t>(y, x) == 255) {
                continue;
            }

            // how do i tell if there is an offset line
            // in between parts of pixels
            x_sum += p.first * w;
            y_sum += p.second * w;
        }
    }
}