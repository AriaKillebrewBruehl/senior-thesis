#include "gray-scale.hpp"
using namespace cv;

cv::Mat grayscale(std::string path, cv::Mat img, bool saving) {
    // opencv image matrix
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in grayscale." << std::endl;
        return image;
    }

    cv::Mat gs;
    gs.create(image.rows, image.cols, CV_8UC1);

    cv::cvtColor(image, gs, cv::COLOR_BGR2GRAY);
    if (saving) {
        save(gs, path, "-gs");
    }

    return gs;
}

// int main(int argc, char** argv) {
//     if (argc < 2) {
//         std::cerr << "Must pass in image to convert to grayscale." << std::endl;
//     } else {
//         // convert files to grayscale  
//         for (int i = 1; i < argc; i++) {
//             try {
//                 cv::Mat image;
//                 grayscale(argv[i], image, true);
//             } catch (const char* msg){
//                 std::cerr << msg << std::endl;
//             }  
//         }
//     }
//     return 0; 
// }