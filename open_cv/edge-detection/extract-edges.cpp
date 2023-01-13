#include "extract-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, int thresh, bool saving) {
    // read images and resize
    cv::Mat image;
    image = read(path, img);
    cv::Mat correct;
    
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in sample." << std::endl;
        return image;
    }
    
    if (image.type() != 0) {
        // convert color image to grayscale
        if (image.type() == 16) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        } 
    
        try{
            if (image.channels() != 1) {
                throw image.channels();
            }
        } catch (int j) {
            std::cout << "ERROR: Input image in sample must be single chanel" << std::endl;
            std::cout << "Input image has " << j << " chanels" << std::endl;
            return image;
        }

        try {
            image.convertTo(correct, CV_8UC1);
            if (correct.type()!= 0) {
                throw image.type();
            }
        } catch (int i) {
            std::cout << "ERROR: Input image in sample could not convert to type CV_8UC1." << std::endl;
            std::cout << "Input image has type " << i << "." << std::endl;
            return image;
        } 
    } else {
        correct = image;
    }

    // int MAX_KERNEL_LENGTH = 15;
    cv::Mat src = correct;
    // // bilateral filter 
    // for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 ) {
    //     cv::Mat dest;
    //     cv::bilateralFilter (src, dest, i, i*2, i/2 );
    //     src = dest;
    // }  

    // run DoG
    // cv::Mat edges = DoG(path, src, false);
    cv::Mat edges = cannyFilter(path, src, false);


    // cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 1));

    // extract edges via threshold
    cv::Mat extracted = threshold("", edges, thresh, false);

    // // morphological operations
    // cv::Mat morphed2;
    // extracted.convertTo(extracted, CV_8UC1);
    // cv::morphologyEx(extracted, morphed2, cv::MORPH_OPEN, element, cv::Point(-1, -1), 3);

    // save image
    if (saving) {
        save(extracted, path, "-extracted");
    }

    return extracted;
}
