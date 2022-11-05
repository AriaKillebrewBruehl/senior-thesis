#include "canny.hpp"

cv::Mat cannyFilter(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);

    cv::Mat dest(image.rows, image.cols, CV_8UC3);
    cv::Canny(image, dest, 100, 200);

    if (saving) {
        save(dest, path, "-canny");
    }
    return dest;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            // image = cv::imread(argv[i], 1);
            cannyFilter(argv[i], image, true);
        }
    }
}