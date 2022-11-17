#include "offset-map.hpp"

cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, std::string pathPriorities, cv::Mat imgPriorities, bool saving) {
    // read images and resize
    cv::Mat dists;
    dists = read(pathDists, imgDists);
    try {
        if (dists.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in offsetMap." << std::endl;
        return dists;
    }
    if (dists.type() != 0) {
        std::cout << "ERROR: Input image must be single chanel in offsetMap." << std::endl;
        return dists;
    }
    cv::Mat priorities;
    priorities = read(pathPriorities, imgPriorities);
    try {
        if (priorities.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in offsetMap." << std::endl;
        return priorities;
    }
    if (priorities.type() != 0) {
        std::cout << "ERROR: Input image must be single chanel in offsetMap." << std::endl;
        return dists;
    }

    cv::Mat offsetMap = cv::Mat::zeros(dists.rows, dists.cols, CV_8UC1);
    // float scalar = 1.5;
    // for (int i = 0; i < offsetMap.rows; i++) {
    //     for (int j = 0; j < offsetMap.cols; j++) { 
    //         int dist = int(dists.at<uchar>(i, j));
    //         if (sin(dist * scalar) >= 0) {
    //             offsetMap.at<uchar>(i, j) = uchar(255);
    //         }
    //     }
    // }



    if (saving) {
        save(offsetMap, pathDists, "-o-map");
    }
    return offsetMap;
}

// int main(int argc, char** argv) {
//     if (argc < 2) {
//          std::cerr << "Must pass in image to run jfa on." << std::endl;
//     } else {
//        for (int i = 1; i < argc; i += 2) {
//             cv::Mat image;
//             offsetMap(argv[i], image, argv[i+1], image, true);
//        }
//     }
// }