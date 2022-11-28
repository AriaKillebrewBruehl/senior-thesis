#include "sampled-transform.hpp"

cv::Mat DT(std::string path, cv::Mat img,bool saving) {
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in distanceMap." << std::endl;
        return image;
    }

    if (saving) {
        save(image, path, "-dt");
    }
}