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

    cv::Mat adjusted = cv::Mat(dists_w_seeds.rows, dists_w_seeds.cols, CV_8UC1,
                               cv::Scalar(255));
    for (auto const &pair : map) {
        int32_t seed_row = pair.first.first;
        int32_t seed_col = pair.first.second;

        float row_sum = 0;
        float col_sum = 0;
        float w = 1.0;
        float ro = 0;

        for (auto const &p : pair.second) {
            int32_t row = p.first;
            int32_t col = p.second;
            if (offset) {
                // if p is part of an offset line
                if (offsets.at<int32_t>(row, col) == 0) {
                    // wi = min{D(xi)/Dw,1}
                    w = dists_w_seeds.at<cv::Vec3i>(row, col)[0] >= 100
                            ? 1.0
                            : float(dists_w_seeds.at<cv::Vec3i>(row, col)[0]) /
                                  100.0;
                } else {
                    // if there is an offset line separating the current pixel
                    // and its seed
                    int32_t seed_section =
                        offsets.at<int32_t>(seed_row, seed_col);
                    int32_t pixel_section = offsets.at<int32_t>(row, col);
                    if (pixel_section != seed_section) {
                        w = 0;
                    }
                }
            } else {
                w = 1.0;
            }

            row_sum += row * w;
            col_sum += col * w;
            ro += w;
        }
        int32_t final_row = int(row_sum / ro);
        int32_t final_col = int(col_sum / ro);
        // std::cout << "prev row: " << seed_row << " prev col: " << seed_col
        //           << std::endl;
        // std::cout << "row sum: " << row_sum << " col sum: " << col_sum
        //           << std::endl;
        // std::cout << "final row: " << final_row << " final col: " <<
        // final_col
        //           << std::endl;
        adjusted.at<uchar>(final_row, final_col) = uchar(0);
    }
    if (saving) {
        save(adjusted, path_offset, "-new-dots");
    }

    return adjusted;
}