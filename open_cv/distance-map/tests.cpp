#include "tests.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i+=2) {
            cv::Mat image;
            // image = read(argv[i], image);
            // cv::bitwise_not(image, image);
            // save(image, argv[i], "-white");
            // sample(image, argv[i], true);

            // jmp_flood2(argv[i], image, true);

            fullMap(argv[i],image, argv[i + 1], image, true);
            // offsetMap(argv[i],image, true);
            // distanceMap(argv[i],image, argv[i + 1], image, true);
        }
    }
}