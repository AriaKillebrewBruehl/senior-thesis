#include "DoG.hpp"

cv::Mat DoG(std::string path, cv::Mat img, bool saving) {
    // read in image
    cv::Mat image;
    image = read(path, img);

    // convert image to grayscale 
    cv::Mat gray_mat(image.size(), CV_8U);
    cv::cvtColor(image, gray_mat, cv::COLOR_BGR2GRAY);

    cv::Mat low_sigma;
    cv::Mat high_sigma;

    cv::GaussianBlur(gray_mat, low_sigma, cv::Size(3, 3), 0, 0);
    cv::GaussianBlur(gray_mat, high_sigma, cv::Size(9, 9), 0, 0);
    // cv::GaussianBlur(gray_mat, low_sigma, cv::Size(0, 0), 1.0, 0);
    // cv::GaussianBlur(gray_mat, high_sigma, cv::Size(0, 0), 1.5, 0);
   
   
    cv::Mat DoG = low_sigma - high_sigma;
    // convert to bi-level image
    cv::Mat image_th;
    double thresh = cv::threshold(DoG, image_th, 10, 255, cv::THRESH_BINARY);

    // save image
    if (saving) {
        save(image_th, path, "-DoG");
    }

    return image_th;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            // image = cv::imread(argv[i], 1);
            DoG(argv[i], image, true);
        }
    }
}