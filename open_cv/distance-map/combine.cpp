#include "combine.hpp"

cv::Mat combine(std::string pathA, cv::Mat imgA, std::string pathB, cv::Mat imgB, bool saving) {
    cv::Mat A, B;
    A = read(pathA, imgA);
    B = read(pathB, imgB);

    cv::Mat combined;
    combined = A + B;

    if (saving) {
        save(combined, pathA, "-combined");
    }

    return combined;
    
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run jfa on." << std::endl;
    } else {
        for (int i = 1; i < argc; i+=2) {
            cv::Mat image;
            combine(argv[i], image, argv[i + 1], image, true);
        }
    }
}