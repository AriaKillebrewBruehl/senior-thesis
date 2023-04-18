#include "match-circles.hpp"

float compare_gray(std::string path, cv::Mat img) {
    image = read(path, img);
    assert(!image.empty());

    if (image.channels() != 1) {
        if (image.type() == 3) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        } else if (image.type() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2GRAY);
        }
    }

    int white = image.rows * image.cols;
    int black = 0;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (image.at<uchar>(i, j) == 0) {
                black++;
                white--;
            }
        }
    }
    float ratio = float(white) / float(image.rows * image.cols);
    return ratio;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i += 2) {
            cv::Mat blank;
            image = read(argv[i], blank);
            assert(!image.empty());

            offset = read(argv[i + 1], blank);
            assert(!offset.empty());
        }
    }

    cv::imshow("Input", image);
    cv::waitKey(0);

    cv::Mat density_map = cv::Mat::zeros(image.size(), CV_16FC1);
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (i > image.rows / 2) {
                density_map.at<float16_t>(i, j) = 4.0 * (1.0 / float(d * d));
            } else {
                density_map.at<float16_t>(i, j) = (1.0 / float(d * d));
            }
        }
    }

    std::cout << "upper density: " << density_map.at<float16_t>(0, 0)
              << std::endl;
    std::cout << "lower density: "
              << density_map.at<float16_t>(density_map.rows - 1, 0)
              << std::endl;
    std::cout << "expected upper seeds: "
              << float(image.rows * image.cols / 2.0) * (1.0 / float(d * d))
              << std::endl;
    std::cout << "expected lower seeds: "
              << float(image.rows * image.cols / 2.0) * (4.0 / float(d * d))
              << std::endl;
    // details = cv::Mat::zeros(offset.size(), offset.type());
    seeds = placeSeeds("", offset, "", density_map, d, false);
    std::cout << "placed seeds" << std::endl;
    cv::imshow("Seeds", seeds);
    cv::waitKey(0);

    adjusted_dots = dots("", offset, "", seeds, d, false);
    std::cout << "adjusted seeds" << std::endl;
    cv::imshow("Adjusted", adjusted_dots);
    cv::waitKey(0);

    // have tone threshold based on percentage of image that contains stipple
    // dots then get max gray value by finding circle radi and computing area of
    // stipple / square density region then to get actual radi look at gray
    // value around the given pixel two decision points, in middle gray area
    // just choose a density and then choose radius based of density

    float T = image.rows * image.cols;
    float density = 1 / float(d * d);
    float num_seeds = T * density;
    float tone = float(image.at<uchar>(0, 0)) / 255.0;
    // float max_size = sqrt(T / num_seeds);
    float gamma = 1.0;
    std::cout << std::pow((1 - tone), gamma) << std::endl;
    // float max_size = sqrt((6 * T * (1 - tone)) / (M_PI * num_seeds));
    // float max_size = sqrt((6 * T) / (M_PI * s));
    float max_size = sqrt((6 * T) / num_seeds) / 2.0;

    stipples = placeDots("", adjusted_dots, "", image, int(max_size), false);
    save(stipples, argv[1], "tone-match");
    std::cout << "placed dots" << std::endl;
    cv::imshow("Stipples", stipples);
    cv::waitKey(0);

    std::cout << "max_size: " << max_size << std::endl;
    std::cout << "size for 50: " << max_size * (1 - (50.0 / 255.0))
              << std::endl;
    std::cout << "size for 125: " << max_size * (1 - (125.0 / 255.0))
              << std::endl;
    std::cout << "size for 200: " << max_size * (1 - (200.0 / 255.0))
              << std::endl;
    float gs = float(image.at<uchar>(0, 0)) / 255.0;
    std::cout << "gray scale: " << gs << std::endl;
    float ratio = compare_gray("", stipples);
    std::cout << "ratio: " << ratio << std::endl;

    cv::waitKey(0);
    return 0;
}