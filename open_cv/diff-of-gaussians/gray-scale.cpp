#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

int grayscale(std::string img) {
    // opencv image matrix
    cv::Mat image;
    // read image
    image = cv::imread(img);
    if (img.empty()) {
        throw "Not a valid image file";
        return -1;
    }
    // generate grayscale image
    for (int i = 0; i < image.rows; i++) {
        for(int j = 0; j < image.cols; j++) {
            unsigned char r = image.at<cv::Vec3b>(i, j)[0];
            unsigned char g = image.at<cv::Vec3b>(i, j)[1];
            unsigned char b = image.at<cv::Vec3b>(i, j)[2];

            unsigned char avg = int((0.299*r + 0.587*g + 0.114*b));
            
            image.at<cv::Vec3b>(i, j)[0] = avg;
            image.at<cv::Vec3b>(i, j)[1] = avg;
            image.at<cv::Vec3b>(i, j)[2] = avg;
        }
    }
    // save image
    std::string output_file = img + "-gs.jpg";
    cv::imwrite(output_file, image);

    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Must pass in image to convert to grayscale." << std::endl;
    } else {
        // convert files to grayscale  
        for (int i = 1; i < argc; i++) {
            try {
                grayscale(argv[i]);
            } catch (const char* msg){
                std::cerr << msg << std::endl;
            }  
        }
    }
    return 0; 
}