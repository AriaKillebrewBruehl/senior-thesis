#include "extract-isophotes.hpp"

cv::Mat extractIsophotes(std::string path, cv::Mat img, int edgethresh,
                         int isothresh, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());

    if (image.type() == 0) {
        if (image.channels() != 1) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }
        image.convertTo(image, 0);
    }

    // get isophote image
    cv::Mat isos = getIsophotes(path, image, isothresh, false);
    assert(isos.type() == 0);

    // extract edges from isophote image
    cv::Mat isosExtracted = extractEdges(path, isos, edgethresh, false);
    assert(isos.type() == 0);

    if (saving) {
        save(isosExtracted, path, "-extracted-isos");
    }

    return isosExtracted;
}
