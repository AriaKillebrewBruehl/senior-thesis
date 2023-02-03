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

cv::Mat adjust(cv::Mat img, std::string path, bool saving) {
    cv::Mat image = read(path, img);

    // TODO type checking
    // given a distance map
    seed_map map = generate_map(image);

    for (auto const &pair : map) {
        pixel_type center = pair.first;
        int32_t x_sum;
        int32_t y_sum;
        int32_t w = 1;
        for (auto const &p : pair.second) {
            x_sum += p.first * w;
            y_sum += p.second * w;
        }
        std::cout << "]}" << std::endl;
    }
}