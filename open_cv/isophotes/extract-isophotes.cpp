#include "extract-isophotes.hpp"

cv::Mat extractIsophotes(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image." << std::endl;
        return image;
    }

    try {
        if (image.type() != 16) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "Input image must be of type 8UC3." << std::endl;
        std::cout << "Provided image was of type " << type2str(t) << "." << std::endl;
        cv::Mat empty;
        return empty;
    }
    

    // get isophote image
    cv::Mat isos = getIsophotes("", image, false);
    
    isos.convertTo(isos, CV_8UC1);
    // extract edges from isophote image
    cv::Mat isosExtracted = extractEdges("", isos, false);

    if (saving) {
        save(isosExtracted, path, "-extracted-isos");
    }

    return isosExtracted;
}

// int main(int argc, char** argv) {
//     if (argc < 2) {
//          std::cerr << "Must pass in image to run DoG on." << std::endl;
//     } else {
//         for (int i = 1; i < argc; i++) {
//             cv::Mat image;
//             extractIsophotes(argv[i],image, true);
//         }
//     }
// }
