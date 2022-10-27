#include "extract-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img) {
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
    cv::Mat dog = DoG(path, image);
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-extracted-DOG" + file_type;
    cv::imwrite(output_file, dog);

    // extract edges via threshold
    cv::Mat extracted = threshold("", dog, 200);
    file_type = path.substr(path.length()-4, 4);
    output_file = path + "-extracted-thresh" + file_type;
    cv::imwrite(output_file, extracted);

    // morphological operations
    cv::Mat morphed;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    std::cout << type2str(extracted.type()) << std::endl;
    extracted.convertTo(extracted, CV_8UC1);
    cv::morphologyEx(extracted, morphed, cv::MORPH_OPEN, element);

    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    file_type = path.substr(path.length()-4, 4);
    output_file = path + "-extracted" + file_type;
    cv::imwrite(output_file, morphed);

    return image;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to extract edges of." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            extractEdges(argv[i], image); 
        }
    }
}