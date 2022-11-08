#include "caboodle.hpp"


cv::Mat caboodle(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);

    if (saving) {
        save(path, image, "-caboodled")
    }

    return image;
}
