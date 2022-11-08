#include "extract-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, bool saving) {
    // read in image
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
    // cv::Mat dog = DoG(path, image, false);
    cv::Mat canny = cannyFilter(path, image, false);

    // cv::Mat morphed;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 1));
    // canny.convertTo(canny, CV_8UC1);
    // cv::morphologyEx(canny, morphed, cv::MORPH_CLOSE, element);
    // cv::Mat morphed3;
    // morphed.convertTo(morphed, CV_8UC1);
    // cv::morphologyEx(morphed, morphed3, cv::MORPH_CLOSE, element);
    // cv::Mat morphed4;
    // morphed3.convertTo(morphed3, CV_8UC1);
    // cv::morphologyEx(morphed3, morphed4, cv::MORPH_CLOSE, element);
    // std::string file_type = path.substr(path.length()-4, 4);
    // std::string output_file = path + "-extracted-pre-thresh-morph" + file_type;
    // cv::imwrite(output_file,  morphed3);


    // extract edges via threshold
    cv::Mat extracted = threshold("", canny, 50, false);
    std::cout << type2str(extracted.type()) << std::endl;

    // morphological operations
    cv::Mat morphed2;
    // cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    extracted.convertTo(extracted, CV_8UC1);
    // save(extracted, path, "-extracted-thresh2");
    cv::morphologyEx(extracted, morphed2, cv::MORPH_OPEN, element, cv::Point(-1, -1), 3);

    // save image
    if (saving) {
        save(morphed2, path, "-extracted");
    }

    return morphed2;
}

// int main(int argc, char** argv) {
//     if (argc < 2) {
//          std::cerr << "Must pass in image to extract edges of." << std::endl;
//     } else {
//         for (int i = 1; i < argc; i++) {
//             cv::Mat image;
//             extractEdges(argv[i], image, true); 
//         }
//     }
// }