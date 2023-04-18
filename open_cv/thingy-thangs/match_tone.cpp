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

    int white = 0;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (image.at<uchar>(i, j) == 255) {
                white++;
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
        for (int i = 1; i < argc; i++) {
            cv::Mat blank;
            image = read(argv[i], blank);
            assert(!image.empty());
        }
    }

    cv::imshow("Input", image);
    cv::waitKey(0);

    cv::Mat match = cv::Mat(image.size(), image.type(), cv::Scalar(255));
    int tone = int(image.at<uchar>(0, 0));
    float ratio = float(tone) / 255.0;
    std::cout << "gray scale: " << ratio << std::endl;
    std::cout << "total_pixels: " << image.rows * image.cols << std::endl;
    int num_white = image.rows * image.cols * ratio;
    int num_black = (image.rows * image.cols) - num_white;
    int num_white_2 = num_white;
    std::cout << "num_white: " << num_white << std::endl;

    cv::RNG rng;
    while (num_black > 0) {
        int col = rng.uniform(0, image.cols + 1);
        int row = rng.uniform(0, image.rows + 1);
        if (match.at<uchar>(row, col) != 0) {
            match.at<uchar>(row, col) = uchar(0);
            num_black--;
        }
    }

    cv::imshow("Match", match);
    cv::waitKey(0);

    float expected_match_ratio =
        float(num_white_2) / float(match.rows * match.cols);

    std::cout << "num_white / (rows * cols): " << expected_match_ratio
              << std::endl;
    float match_ratio = compare_gray("", match);
    std::cout << "match_ratio: " << match_ratio << std::endl;

    cv::waitKey(0);
    return 0;
}