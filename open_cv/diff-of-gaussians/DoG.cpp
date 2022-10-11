#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>


cv::Mat grayscale(cv::Mat image) {
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

    return image;
}


int erode_dilate(std::string img) {
    cv::Mat image;
    // read image
    image = cv::imread(img);
    if (img.empty()) {
        throw "Not a valid image file.";
        return 1;
    }

    cv::Mat opened;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));

    for (int i = 0; i < 5; i++) {
        cv::morphologyEx(image, opened, cv::MORPH_OPEN, element);
         // save image
        std::string file_type = img.substr(img.length()-4, 4);
        std::string output_file_morphed = img + "-morphed-" + std::to_string(i) + file_type;
        cv::imwrite(output_file_morphed, opened);
        image = opened;
    }
    
    return 0;
}

int DoG(std::string img) {
    cv::Mat image;
    // read image
    image = cv::imread(img);
    if (img.empty()) {
        throw "Not a valid image file.";
        return 1;
    }

    // convert image to grayscale 
    image = grayscale(image);

    cv::Mat low_sigma;
    cv::Mat high_sigma;

    cv::GaussianBlur(image, low_sigma, cv::Size(3, 3), 0, 0);
    cv::GaussianBlur(image, high_sigma, cv::Size(9, 9), 0, 0);

    // cv::GaussianBlur(image, low_sigma, cv::Size(0, 0), 1.0, 0);
    // cv::GaussianBlur(image, high_sigma, cv::Size(0, 0), 1.5, 0);
   
    cv::Mat DoG = low_sigma - high_sigma;
    // convert to bi-level image
    double thresh = cv::threshold(DoG, DoG, 10, 255, cv::THRESH_BINARY);

    // save image
    std::string file_type = img.substr(img.length()-4, 4);
    std::string output_file = img + "-DoG" + file_type;
    cv::imwrite(output_file, DoG);

    erode_dilate(output_file);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            erode_dilate(argv[i]);
        }
    }
}