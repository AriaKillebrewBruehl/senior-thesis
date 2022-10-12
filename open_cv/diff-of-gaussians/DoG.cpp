# include "DoG.hpp"

cv::Mat DoG(std::string img) {
    cv::Mat image;
    // read image
    image = cv::imread(img, 1);
    if (image.empty()) {
        throw "Not a valid image file.";
        cv::Mat empty;
        return empty;
    }

    // convert image to grayscale 
    cv::Mat gray_mat(image.size(), CV_8U);
    cv::cvtColor(image, gray_mat, cv::COLOR_BGR2GRAY);

    cv::Mat low_sigma;
    cv::Mat high_sigma;

    cv::GaussianBlur(gray_mat, low_sigma, cv::Size(3, 3), 0, 0);
    cv::GaussianBlur(gray_mat, high_sigma, cv::Size(9, 9), 0, 0);
   
    cv::Mat DoG = low_sigma - high_sigma;
    // convert to bi-level image
    cv::Mat image_th;
    cv::Mat bin_mat(DoG.size(), DoG.type());

    double thresh = cv::threshold(DoG, image_th, 10, 255, cv::THRESH_BINARY);

    // save image
    std::string file_type = img.substr(img.length()-4, 4);
    std::string output_file = img + "-DoG" + file_type;
    cv::imwrite(output_file, image_th);

    return image_th;
}


int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            DoG(argv[i]);
        }
    }
}