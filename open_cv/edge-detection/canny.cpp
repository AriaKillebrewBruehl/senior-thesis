#include "canny.hpp"

cv::Mat cannyFilter(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    if (image.type() != 0) {
        throw "Image must be single chanel grayscale image.";
        return image;
    }

    cv::Mat dst, edges;

    int threshold = 40;
    const int ratio = 3;
    const int kernel_size = 3;
    
    dst.create(image.size(), image.type());

    float sigma = 0.33;

    double m = cv::mean(image)[0];

    int lower = int(std::max(0.0, (1.0 - sigma) * m));
    int upper = int(std::min(255.0, (1.0 + sigma) * m));
    // blur image to remove noise
    cv::blur(image, edges, cv::Size(3, 3));

    // apply Canny filter
    cv::Canny(edges, edges, lower, upper, kernel_size);

//    dst = cv::Scalar::all(0);
//    image.copyTo(dst, edges);

    if (saving) {
        save(edges, path, "-canny-40");
    }
    return edges;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            cannyFilter(argv[i], image, true);
        }
    }
}