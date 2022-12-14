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
        if (image.type() != 16 && image.type() != 0) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "ERROR: Input image in caboodle must be of type 8UC3 or 8UC1." << std::endl;
        std::cout << "ERROR: Provided image was of type " << t << ", " << type2str(t) << "." << std::endl;
        cv::Mat empty;
        return empty;
    }

    // step 0: extract grayscale version of image
    cv::Mat gray;
    if (image.type() != 0) {
        gray = grayscale(path, image, true);
        std::cout << "converted image to grayscale" << std::endl;
    } else {
        gray = image;
    }

    // step 1: extract the edges of the image
    cv::Mat edges = extractEdges(path, gray, 1000, true);
    assert(edges.type() == 0);
    std::cout << "extracted edges from image" << std::endl;
    
    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =  cv::connectedComponentsWithStats(edges, labels, stats, centroids);

    int i;
    int j;
    for (i = 0; i < labels.rows; i++) {
        for (j = 0; j < labels.cols; j++) {
            if (labels.at<int>(i, j) == 0) {
                break;
            }
        }
    }

    // background is black so invert
    if (edges.at<int>(i,j) == 0) {
        cv::bitwise_not(edges, edges);
    }
    

    // step 2: extract the isophotes of the image
    cv::Mat isophotes = extractIsophotes(path, image, 150, 5, true);
    assert(isophotes.type() == 0);
    std::cout << "extracted isophotes from image" << std::endl;

    // get components
    numComps =  cv::connectedComponentsWithStats(isophotes, labels, stats, centroids);

    for (i = 0; i < labels.rows; i++) {
        for (j = 0; j < labels.cols; j++) {
            if (labels.at<int>(i, j) == 0) {
                break;
            }
        }
    }

    // background is black so invert
    if (isophotes.at<int>(i,j) == 0) {
        cv::bitwise_not(isophotes, isophotes);
    }
    
    // step 3: offset map
    cv::Mat map = fullMap(path, edges, path, isophotes, saving);
    std::cout << "extracted distance map from image" << std::endl;

    return map;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to caboodle." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            caboodle(argv[i], image, true); 
        }
    }
}
