#include "DoG.hpp"

cv::Mat DoG(std::string path, cv::Mat img, bool saving) {
    // read in image
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in DoG." << std::endl;
        return image;
    }

    try {
        if (image.type() != 0) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "ERROR: Input image to DoG must be of type 8UC1."
                  << std::endl;
        std::cout << "ERROR: Provided image was of type " << type2str(t) << "."
                  << std::endl;
        cv::Mat empty;
        return empty;
    }

    cv::Mat low_sigma;
    cv::Mat high_sigma;

    cv::GaussianBlur(image, low_sigma, cv::Size(3, 3), 0, 0);
    cv::GaussianBlur(image, high_sigma, cv::Size(9, 9), 0, 0);

    cv::Mat DoG = low_sigma - high_sigma;
    // convert to bi-level image
    double thresh = cv::threshold(DoG, DoG, 0, 255, cv::THRESH_BINARY);

    // save image
    if (saving) {
        save(DoG, path, "-DoG10");
    }

    return DoG;
}