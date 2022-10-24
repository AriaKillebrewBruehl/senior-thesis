#include "isophotes.hpp"

cv::Mat getIsophotes(std::string path, cv::Mat img) {
    cv::Mat image;
    // read in image
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read in image unchanged
        image = cv::imread(path, 0);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }
        // std::cout << type2str(image.type()) << std::endl;
        // if (image.type() != 0) {
        //     throw "Image must be of type 0 (8UC1)";
        //     return image;
        // }
    } else if (!img.empty()) {
        image = img;
        // if (image.type() != 0) {
        //     throw "Image must be of type 0 (8UC1)";
        //     return image;
        // }
    }

    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-iso" + file_type;
    cv::imwrite(output_file, image);
    return image;
}