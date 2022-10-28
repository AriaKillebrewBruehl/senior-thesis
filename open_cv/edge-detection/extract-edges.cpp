#include "extract-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, bool saving) {
    // read in image
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read image as grayscale
        image = cv::imread(path, 1);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }
    } else if (!img.empty()) {
        image = img;
    }

    // run DoG
    cv::Mat dog = DoG(path, image, false);

    cv::Mat morphed;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    dog.convertTo(dog, CV_8UC1);
    cv::morphologyEx(dog, morphed, cv::MORPH_CLOSE, element);
    cv::Mat morphed3;
    morphed.convertTo(morphed, CV_8UC1);
    cv::morphologyEx(morphed, morphed3, cv::MORPH_CLOSE, element);
    cv::Mat morphed4;
    morphed3.convertTo(morphed3, CV_8UC1);
    cv::morphologyEx(morphed3, morphed4, cv::MORPH_CLOSE, element);
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-extracted-pre-thresh-morph" + file_type;
    cv::imwrite(output_file,  morphed3);


    // extract edges via threshold
    cv::Mat extracted = threshold("", morphed, 200, false);
    file_type = path.substr(path.length()-4, 4);
    output_file = path + "-extracted-thresh" + file_type;
    cv::imwrite(output_file, extracted);

    // morphological operations
    cv::Mat morphed2;
    // cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    extracted.convertTo(extracted, CV_8UC1);
    cv::morphologyEx(extracted, morphed2, cv::MORPH_OPEN, element);

    // save image
    if (saving) {
        save(morphed2, path, "-extracted");
    }

    return image;
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