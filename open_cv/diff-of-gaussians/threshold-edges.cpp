#include "threshold-edges.hpp"

cv::Mat threshold(std::string path, cv::Mat img) {
    // read in image
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read image
        image = cv::imread(path, 0);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }   
    } else if (!img.empty()) {
        image = img;
    }
    // convert to binary 
    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);

    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;

    int numComps =  cv::connectedComponentsWithStats(image, labels, stats, centroids); 	

    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-skel" + file_type;
    cv::imwrite(output_file, skel);

    return image
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            image = cv::imread(argv[i], 0);
        }
    }
}