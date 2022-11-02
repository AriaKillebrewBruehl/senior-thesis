#include "distance-map.hpp"

cv::Mat distanceMap(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);

   // save image
    if (saving) {
        save(image, path, "-jfa");
    }
    return image;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run jfa on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            distanceMap(argv[i], image, true);
        }
    }
}