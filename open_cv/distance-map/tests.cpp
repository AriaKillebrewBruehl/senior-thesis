#include "tests.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i+=1) {
            cv::Mat image;
            sample_seeds(image, argv[i], true, false);
        }
    }
}