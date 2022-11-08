#include "extract-isophotes.hpp"

cv::Mat extractIsophotes(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);

    if (image.type() != 16) {
        throw "Input image must be of type 8UC3.";
        return image;
    }

    // get isophote image
    cv::Mat isos = getIsophotes("", image, false);
    // extract edges from isophote image
    cv::Mat isosExtracted = extractEdges("", isos, false);

    if (saving) {
        save(isosExtracted, path, "-extracted-isos");
    }

    return isosExtracted;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            extractIsophotes(argv[i],image, true);
        }
    }
}
