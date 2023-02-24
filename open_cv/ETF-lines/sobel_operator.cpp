#include "sobel_operator.hpp"

cv::Mat sobel_mag_angle(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    assert(!image.empty());

    if (image.type() != 16) {
        if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
        } else if (image.channels() == 1) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
        }
        image.convertTo(image, 16);
    }

    // First we declare the variables we are going to use
    cv::Mat src, src_gray;
    cv::Mat grad;
    int ddepth = CV_16S;
    int ksize = 1;
    int scale = 1;
    int delta = 0;

    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    cv::GaussianBlur(image, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
    // Convert the image to grayscale
    cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    // get gradient in x direction
    cv::Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta,
              cv::BORDER_DEFAULT);
    // get gradient in y direction
    cv::Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta,
              cv::BORDER_DEFAULT);
    std::cout << grad_x << std::endl;
    std::cout << grad_y << std::endl;
    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, grad_x);
    cv::convertScaleAbs(grad_y, grad_y);

    cv::Mat g_zero = cv::Mat(grad_x.size(), CV_8UC2);
    // cv::Mat sobelMagnitude = cv::Mat::zeros(grad_x.size(), CV_16FC1);
    // cv::Mat sobelAngles = cv::Mat::zeros(grad_x.size(), CV_16FC1);
    for (int i = 0; i < grad_x.rows; i++) {
        for (int j = 0; j < grad_x.cols; j++) {
            float x = (float)grad_x.at<uchar>(i, j);
            float y = (float)grad_y.at<uchar>(i, j);
            float mag = sqrt(x * x + y * y);
            g_zero.at<cv::Vec2b>(i, j)[0] = mag;
            float theta = atan(y / x);
            g_zero.at<cv::Vec2b>(i, j)[1] = theta;
        }
    }

    if (saving) {
        save(g_zero, path, "-sobel-mag-dir");
    }

    std::cout << g_zero << std::endl << std::endl;

    return g_zero;
}