#include "place-dots.hpp"

cv::Mat placeDots(std::string path, cv::Mat offsetMap, bool saving) {
    cv::Mat image;
    image = read(path, offsetMap);


    if (saving) {
        save(image, path, "-dots");
    }

    return image;
}