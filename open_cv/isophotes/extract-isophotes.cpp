#include "extract-isophotes.hpp"

cv::Mat extractIsophotes(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in extractIsophotes." << std::endl;
        return image;
    }

    try {
        if (image.type() == 0) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
        }
        if (image.type() != 16) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "Input image in extractIsophotes must be of type 8UC3." << std::endl;
        std::cout << "Provided image was of type " << type2str(t) << "." << std::endl;
        cv::Mat empty;
        return empty;
    }

    // get isophote image
    cv::Mat isos = getIsophotes(path, image, false);
    assert(isos.type() == 0);
    
    // extract edges from isophote image
    cv::Mat isosExtracted = extractEdges(path, isos, false);
    assert(isos.type() == 0);

    if (saving) {
        save(isosExtracted, path, "-extracted-isos");
    }

    return isosExtracted;
}
