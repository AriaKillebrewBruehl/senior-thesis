#include "extract-edges.hpp"


cv::Mat morph_open(std::string file_path, cv::Mat image) {
    cv::Mat opened;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));

    cv::morphologyEx(image, opened, cv::MORPH_OPEN, element);
    // save image
    std::string file_type = file_path.substr(file_path.length()-4, 4);
    std::string output_file_morphed = file_path + "-morphed-"+ file_type;
    cv::imwrite(output_file_morphed, opened);
    
    return opened;
}




cv::Mat extractEdges(std::string path, cv::Mat img) {
     // read in image
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read image
        image = cv::imread(path, 0);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }
    } else if (!img.empty()) {
        image = img;
    }

    // run DoG
    cv::Mat dog = DoG(path, image);

    // extract edges via threshold
    cv::Mat extracted = threshold("", dog, 100);

    // morphological operations
    cv::Mat morphed;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));

    cv::morphologyEx(extracted, morphed, cv::MORPH_OPEN, element);


    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-thresh" + file_type;
    cv::imwrite(output_file, image);

    return image;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            threshold(argv[i], image, 50); 
        }
    }
}