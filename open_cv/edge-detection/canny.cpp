#include "canny.hpp"

cv::Mat cannyFilter(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);


    if (saving) {
        save(image, path, "-canny");
    }
    return image;
}