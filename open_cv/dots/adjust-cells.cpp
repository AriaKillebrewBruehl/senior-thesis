#include "adjust-cells.hpp"

seed_map generate_map(cv::Mat image) {
    assert(!image.empty());
    assert(image.channels() == 3);
    seed_map map{};

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (image.at<int32_t>(i, j) != 255) {
                pixel_type seed;
                seed.first = image.at<cv::Vec3i>(i, j)[1];
                seed.second = image.at<cv::Vec3i>(i, j)[2];

                pixel_type current;
                current.first = i;
                current.second = j;
                map[seed].push_back(current);
            }
        }
    }
    return map;
}

cv::Mat adjust(std::string path_offset, cv::Mat img_offset,
               std::string path_seeds, cv::Mat img_seeds, bool saving,
               bool offset) {
    cv::Mat offsets = read(path_offset, img_offset);
    assert(!offsets.empty());
    if (offsets.type() != 4) {
        if (offsets.channels() != 1) {
            cv::cvtColor(offsets, offsets, cv::COLOR_RGB2GRAY);
        }
        offsets.convertTo(offsets, CV_32SC1);
    }

    cv::Mat seeds = read(path_seeds, img_seeds);
    assert(!seeds.empty());
    assert(seeds.type() == 20);

    // given a distance map
    seed_map map = generate_map(seeds);

    cv::Mat adjusted = cv::Mat::zeros(seeds.rows, seeds.cols, CV_32SC1);
    int i = 0;
    for (auto const &pair : map) {
        int32_t x_sum = 0;
        int32_t y_sum = 0;
        float w = 1.0;
        int32_t ro = 0;
        for (auto const &p : pair.second) {
            int32_t x = p.first;
            int32_t y = p.second;
            if (offset) {
                // if p is part of an offset line don't include it
                if (offsets.at<int32_t>(y, x) == 255) {
                    // wi = min{D(xi)/Dw,1}
                    // what is D(xi) the value of the pixel in the distance map
                    continue;
                }

                // how do i tell if there is an offset line
                // in between parts of pixels
                // label all the connected regions that are between the offset
                // lines if the region of the centroid is different from the
                // region of the current pixel, don't include it
            }
            x_sum += x * w;
            y_sum += x * w;
            ro += w;
        }
        int32_t final_x = x_sum / ro;
        int32_t final_y = y_sum / ro;
        adjusted.at<int32_t>(final_x, final_y) = int32_t(255);
        i++;
        // std::cout << "prev center: " << pair.first.first << ", "
        //           << pair.first.second << std::endl;
        // std::cout << "new center:  " << final_x << ", " << final_y <<
        // std::endl; std::cout << std::endl;
    }
    if (saving) {
        save(adjusted, path_offset, "-new-dots");
    }

    return adjusted;
}