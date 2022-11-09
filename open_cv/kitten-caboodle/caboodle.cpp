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
    std::cout << type2str(image.type()) << std::endl;
    // step 1: extract the edges of the image
    cv::Mat edges = extractEdges("", image, false);

    // step 2: extract the isophotes of the image
    cv::Mat isophotes = extractIsophotes("", image, false);

    // step 3: distance map
    distMap map = distanceMap("", edges, "", isophotes, false);

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
