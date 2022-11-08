#include "read-save.hpp"

cv::Mat read(std::string path, cv::Mat img) {
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        image = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }
    } else if (!img.empty()) {
        image = img;
    }

    return image;
}

void save(cv::Mat image, std::string path, std::string suffix) {
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + suffix + file_type;
    cv::imwrite(output_file, image);

    return;
}