#include "caboodle.hpp"


cv::Mat caboodle(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
     try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in caboodle." << std::endl;
        return image;
    }

    try {
        if (image.type() != 16) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "ERROR: Input image in caboodle must be of type 8UC3." << std::endl;
        std::cout << "ERROR: Provided image was of type " << type2str(t) << "." << std::endl;
        cv::Mat empty;
        return empty;
    }

    // step 0: extract grayscale version of image
    cv::Mat gray = grayscale("", image, false);
    std::cout << "converted image to grayscale" << std::endl;

    // step 1: extract the edges of the image
    cv::Mat edges = extractEdges("", gray, true);
    std::cout << "extracted edges from image" << std::endl;

    // step 2: extract the isophotes of the image
    cv::Mat isophotes = extractIsophotes("", image, true);
    std::cout << "extracted isophotes from image" << std::endl;

    // step 3: distance map
    distMap map = distanceMap("", edges, "", isophotes, false);
    std::cout << "extracted distance map from image" << std::endl;
    cv::Mat distances = map.distances;

    if (saving) {
        save(distances, path, "-caboodled");
    }

    return distances;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to extract edges of." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            caboodle(argv[i], image, true); 
        }
    }
}
