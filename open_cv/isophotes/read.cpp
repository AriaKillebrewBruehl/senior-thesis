#include <string>
#include <opencv2/opencv.hpp>

cv::Mat read(std::string path, cv::Mat img) {
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        image = cv::imread(path);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }
    } else if (!img.empty()) {
        image = img;
    }

    return image;
}