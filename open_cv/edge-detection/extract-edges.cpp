#include "extract-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, int thresh, bool saving) {
    // read in image
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in extractEdges." << std::endl;
        return image;
    }
    try {
        if (image.type() != 0) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "ERROR: Input image to extractEdges must be of type 8UC1." << std::endl;
        std::cout << "ERROR: Provided image was of type " << type2str(t) << "." << std::endl;
        cv::Mat empty;
        return empty;
    }

    // run DoG
    cv::Mat edges = DoG(path, image, false);
    // cv::Mat edges = cannyFilter(path, image, false);


    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 1));

    // extract edges via threshold
    cv::Mat extracted = threshold("", edges, thresh, false);

    // morphological operations
    cv::Mat morphed2;
    extracted.convertTo(extracted, CV_8UC1);
    cv::morphologyEx(extracted, morphed2, cv::MORPH_OPEN, element, cv::Point(-1, -1), 3);

    // save image
    if (saving) {
        save(morphed2, path, "-extracted");
    }

    return morphed2;
}
