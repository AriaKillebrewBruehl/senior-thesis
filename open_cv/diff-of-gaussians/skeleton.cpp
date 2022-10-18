#include "skeleton.hpp"

cv::Mat skeleton(std::string path, cv::Mat img) {
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
    // convert to binary 
    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);


    // save image
    if (path == "") {
        int rand = std::rand() % 100;
        path = "./" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-skel" + file_type;
    cv::imwrite(output_file, image);

    return image;
     

}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat img;
            skeleton(argv[i],img );
        }
    }
}