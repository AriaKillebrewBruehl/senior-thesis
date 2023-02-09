#include "adjust-cells.hpp"

seed_map generate_map(cv::Mat image) {
    assert(!image.empty());
    assert(image.channels() == 3);
    seed_map map{};

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            // we aren't at a seed pixel
            if (image.at<cv::Vec3i>(i, j)[0] != 0) {
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
    // for (auto pair : map) {
    //     std::cout << "(" << pair.first.first << ", " << pair.first.second
    //               << "): [";
    //     for (auto p : pair.second) {
    //         std::cout << "(" << p.first << ", " << p.second << "), ";
    //     }
    //     std::cout << "]" << std::endl;
    //     std::cout << std::endl;
    // }
    return map;
}

cv::Mat adjust(std::string path_offset, cv::Mat img_offset,
               std::string path_dists_w_seeds, cv::Mat img_dists_w_seeds,
               bool saving, bool offset) {
    cv::Mat offsets = read(path_offset, img_offset);
    assert(!offsets.empty());
    if (offsets.type() != 4) {
        if (offsets.channels() != 1) {
            cv::cvtColor(offsets, offsets, cv::COLOR_RGB2GRAY);
        }
        offsets.convertTo(offsets, CV_32SC1);
    }

    cv::Mat dists_w_seeds = read(path_dists_w_seeds, img_dists_w_seeds);
    assert(!dists_w_seeds.empty());
    assert(dists_w_seeds.type() == 20);

    seed_map map = generate_map(dists_w_seeds);

    cv::Mat adjusted = cv::Mat(dists_w_seeds.rows, dists_w_seeds.cols, CV_32SC1,
                               cv::Scalar(255));
    for (auto const &pair : map) {
        int32_t seed_x = pair.first.first;
        int32_t seed_y = pair.first.second;
        float x_sum = 0;
        float y_sum = 0;
        float w = 1.0;
        float ro = 0;
        for (auto const &p : pair.second) {
            int32_t x = p.first;
            int32_t y = p.second;
            if (offset) {
                // if p is part of an offset line
                if (offsets.at<int32_t>(y, x) == 0) {
                    // wi = min{D(xi)/Dw,1}
                    w = dists_w_seeds.at<cv::Vec3i>(y, x)[0] >= 100
                            ? 1.0
                            : float(dists_w_seeds.at<cv::Vec3i>(y, x)[0]) /
                                  100.0;
                }
                // if there is an offset line separating the current pixel and
                // its seed
                int seed_section = offsets.at<int32_t>(seed_y, seed_x);
                int pixel_section = offsets.at<int32_t>(y, x);
                if (pixel_section != seed_section) {
                    w = 0;
                }
            } else {
                w = 1.0;
            }
            x_sum += x * w;
            y_sum += x * w;
            ro += w;
        }
        int32_t final_x = x_sum / ro;
        int32_t final_y = y_sum / ro;
        // std::cout << "prev x: " << seed_x << " prev y: " << seed_y <<
        // std::endl; std::cout << "x sum: " << x_sum << " y sum: " << y_sum <<
        // std::endl; std::cout << "final x: " << final_x << " final y: " <<
        // final_y
        //           << std::endl;
        adjusted.at<int32_t>(final_x, final_y) = 0;
    }
    if (saving) {
        save(adjusted, path_offset, "-new-dots");
    }

    return adjusted;
}