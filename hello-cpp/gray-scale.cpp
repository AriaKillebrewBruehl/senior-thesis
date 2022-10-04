#include "gray-scale.hpp"

using namespace cv;

int main() {
    cv::Mat image; // image matrix 
    image = cv::imread("images/flowers.jpg");
    for (int i = 0; i < image.rows; i++) {
        for(int j = 0; j < image.cols; j++) {
            unsigned char r = image.at<cv::Vec3b>(i, j)[0];
            unsigned char g = image.at<cv::Vec3b>(i, j)[1];
            unsigned char b = image.at<cv::Vec3b>(i, j)[2];

            unsigned char avg = int((0.299*r + 0.587*g + 0.114*b));
            
            image.at<cv::Vec3b>(i, j)[1] = avg;
            image.at<cv::Vec3b>(i, j)[2] = avg;
            image.at<cv::Vec3b>(i, j)[0] = avg;
        }
    }
    cv::imwrite("images/flowers.jpg-gs.jpg", image);
}