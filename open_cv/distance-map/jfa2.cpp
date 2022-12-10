#include "jfa.hpp"

seed_map get_seeds2(cv::Mat img) {
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
    cv::threshold(gray, gray, 0, 255,  cv::THRESH_BINARY);
    save(gray, "", "-jfa-thresh");
    // seeds[(i, j)] gives the seed pixel for (i, j)
    seed_map seeds;
    
    for (int i = 0; i < gray.rows; i++) {
        for(int j = 0; j < gray.cols; j++) {
            pixel_type pix(i, j);
            uchar r = gray.at<uchar>(i, j);

            // the pixel is undefined
            if (int(r) == 0) {
                seeds[pix] = undef;
            } else {
                seeds[pix] = pix;
            }
        }
    }
    return seeds;
}

/*seed_map jmp_flood_seeds(std::string path, cv::Mat img) {
    seed_map map;
    cv::Mat image;
    // read image and resize
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in jmp_flood." << std::endl;
        return map;
    }
    resz(image);

    seed_map seeds = get_seeds(image);

    int N = image.cols;
    int k = 1;

   while (N/k >= 1) {
        std::cout << N/k << std::endl;
        // loop over image
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                pixel_type p(i, j);
                // loop over 9 neighbors 
                std::vector<int> nbrs_i = {i - k, i, i + k};
                for (int qi : nbrs_i) {
                    // bounds check
                    if (qi < 0 || qi > N - 1) {
                        continue;
                    }
                    std::vector<int> nbrs_j = {j - k, j, j + k};
                    for (int qj : nbrs_j) {
                        // bounds check 
                        if (qj < 0 || qj > N - 1) {
                            continue;
                        } 
                        else if (i == qi && j == qj) {
                            continue;
                        }
                        pixel_type q(qi, qj);

                        // q has no seed
                        if (seeds[q] == undef) {
                            continue;
                        }
                        else {
                            unsigned char qr = image.at<cv::Vec3b>(qi, qj)[0]; 
                            unsigned char qg = image.at<cv::Vec3b>(qi, qj)[1]; 
                            unsigned char qb = image.at<cv::Vec3b>(qi, qj)[2]; 
                            // p is undefined, so set to q
                            if (seeds[p] == undef) {
                                seeds[p] = seeds[q];
                            }
                            // both p and q are colored  
                            else {
                                pixel_type p_seed = seeds[p];
                                pixel_type q_seed = seeds[q];
                                if (p_seed == q_seed) {
                                    continue;
                                }
                                double dist_p_seed = std::hypot(p.first - p_seed.first, p.second - p_seed.second);
                                double dist_q_seed = std::hypot(p.first - q_seed.first, p.second - q_seed.second);
                                // p is closer to q's seed than its own
                                if (dist_p_seed > dist_q_seed) {
                                    seeds[p] = seeds[q];
                                }
                            }
                        }
                    }
                }
            }
        }
        k *= 2;
    }

    return seeds;
}*/

cv::Mat jmp_flood2(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    // read image and resize
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in jmp_flood." << std::endl;
        return image;
    }
    resz(image);

    seed_map seeds = get_seeds2(image);

    int N = image.cols;
    int k = 1;

   while (N/k >= 1) {
        std::cout << N/k << std::endl;
        // loop over image
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                pixel_type p(i, j);
                // loop over 9 neighbors 
                std::vector<int> nbrs_i = {i - k, i, i + k};
                for (int qi : nbrs_i) {
                    // bounds check
                    if (qi < 0 || qi > N - 1) {
                        continue;
                    }
                    std::vector<int> nbrs_j = {j - k, j, j + k};
                    for (int qj : nbrs_j) {
                        // bounds check 
                        if (qj < 0 || qj > N - 1) {
                            continue;
                        } 
                        else if (i == qi && j == qj) {
                            continue;
                        }
                        pixel_type q(qi, qj);

                        // q has no seed
                        if (seeds[q] == undef) {
                            continue;
                        }
                        else {
                            unsigned char qr = image.at<cv::Vec3b>(qi, qj)[0]; 
                            unsigned char qg = image.at<cv::Vec3b>(qi, qj)[1]; 
                            unsigned char qb = image.at<cv::Vec3b>(qi, qj)[2]; 
                            // p is undefined, so set to q
                            if (seeds[p] == undef) {
                                // set p to be the color of its q
                                image.at<cv::Vec3b>(i, j)[0] = qr;
                                image.at<cv::Vec3b>(i, j)[1] = qg;
                                image.at<cv::Vec3b>(i, j)[2] = qb;
                                // update pixel map
                                seeds[p] = seeds[q];
                            }
                            // both p and q are colored  
                            else {
                                pixel_type p_seed = seeds[p];
                                pixel_type q_seed = seeds[q];
                                if (p_seed == q_seed) {
                                    continue;
                                }
                                double dist_p_seed = std::hypot(p.first - p_seed.first, p.second - p_seed.second);
                                double dist_q_seed = std::hypot(p.first - q_seed.first, p.second - q_seed.second);
                                // p is closer to q's seed than its own
                                // if (abs(dist_p_seed - dist_q_seed) <= 0.5) {
                                //     int p_id = p_seed.first * image.rows + p_seed.second * image.cols;
                                //     int q_id = q_seed.first * image.rows + q_seed.second * image.cols;
                                //     if (q_id < p_id) {
                                //         // set p to be the color of q
                                //         image.at<cv::Vec3b>(i, j)[0] = qr;
                                //         image.at<cv::Vec3b>(i, j)[1] = qg;
                                //         image.at<cv::Vec3b>(i, j)[2] = qb;
                                //         // update pixel map
                                //         seeds[p] = seeds[q];
                                //     }
                                // if the distance is the same pick a random winner
                                // if (dist_p_seed == dist_q_seed) {
                                //     if (rand() % 2) {
                                //          // set p to be the color of q
                                //         image.at<cv::Vec3b>(i, j)[0] = qr;
                                //         image.at<cv::Vec3b>(i, j)[1] = qg;
                                //         image.at<cv::Vec3b>(i, j)[2] = qb;
                                //         // update pixel map
                                //         seeds[p] = seeds[q];
                                //     }
                                if (dist_p_seed > dist_q_seed) {
                                    // set p to be the color of q
                                    image.at<cv::Vec3b>(i, j)[0] = qr;
                                    image.at<cv::Vec3b>(i, j)[1] = qg;
                                    image.at<cv::Vec3b>(i, j)[2] = qb;
                                    // update pixel map
                                    seeds[p] = seeds[q];
                                }
                            }
                        }
                    }
                }
            }
        }
        k *= 2;
    }

    if (saving) {
        save(image, path, "-jfa-2");
    }
    return image;
}