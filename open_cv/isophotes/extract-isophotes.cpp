#include "extract-isophotes.hpp"

cv::Mat extractIsophotes(std::string path, cv::Mat img, int edgethresh,
                         int isothresh, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());

    if (image.type() != 16) {
        if (image.channels() != 3) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
        }
        image.convertTo(image, 16);
    }

    save(image, path, "-isos-input");

    // get isophote image
    cv::Mat isos = getIsophotes(path, image, isothresh, true);
    assert(isos.type() == 0);

    // extract edges from isophote image
    cv::Mat isosExtracted = extractEdges(path, isos, edgethresh, false);
    assert(isos.type() == 0);

    if (saving) {
        save(isosExtracted, path, "-extracted-isos");
    }

    return isosExtracted;
}
