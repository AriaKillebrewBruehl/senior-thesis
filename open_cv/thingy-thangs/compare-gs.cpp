#include "compare-gs.hpp"

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

    details = cv::Mat::zeros(offset.size(), offset.type());
    seeds = placeSeeds("", offset, "", details, lane_size, false);
    std::cout << "placed seeds" << std::endl;
    cv::imshow("Seeds", seeds);
    cv::waitKey(0);

    adjusted_dots = dots("", offset, "", seeds, lane_size, false);
    std::cout << "adjusted seeds" << std::endl;
    cv::imshow("Adjusted", adjusted_dots);
    cv::waitKey(0);

    // float density = 1 / float(lane_size * lane_size);
    // int total_seeds = (seeds.rows * seeds.cols) * density;

    float density = 1 / float((lane_size) * (lane_size));
    max_size = sqrt((1 / density));
    stipples = placeDots("", seeds, "", image, max_size, false);
    save(stipples, argv[1], "tone-match");
    std::cout << "placed dots" << std::endl;
    cv::imshow("Stipples", stipples);
    cv::waitKey(0);

    float gs = float(image.at<uchar>(0, 0)) / 255.0;
    std::cout << "gray scale: " << gs << std::endl;
    float ratio = compare_gray("", stipples);
    std::cout << "ratio: " << ratio << std::endl;

    return 0;
}