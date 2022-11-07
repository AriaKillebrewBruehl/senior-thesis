#include "extract-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, bool saving) {
    // read in image
    cv::Mat image;
    image = read(path, img);
    if (image.type() != 0) {
        throw "Image must be single chanel grayscale image";
    }

    // run DoG
    // cv::Mat dog = DoG(path, image, false);
    cv::Mat canny = cannyFilter(path, image, false);

    // cv::Mat morphed;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
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

    // morphological operations
    cv::Mat morphed2;
    // cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    extracted.convertTo(extracted, CV_8UC1);
    cv::morphologyEx(extracted, morphed2, cv::MORPH_OPEN, element);

    // save image
    if (saving) {
        save(morphed2, path, "-extracted");
    }

    return morphed2;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to extract edges of." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            extractEdges(argv[i], image, true); 
        }
    }
}