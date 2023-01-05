#include "tests.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run isophotes tests on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            extractIsophotes(argv[i],image, 150, 4, true);
        }
    }
}