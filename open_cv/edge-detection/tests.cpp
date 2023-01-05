#include "tests.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            extractEdges(argv[i], image, 1000, true);
            // grayscale(argv[i], image, true);
        }
    }
}