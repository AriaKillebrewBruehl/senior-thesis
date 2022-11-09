#include "canny.hpp"

cv::Mat cannyFilter(std::string path, cv::Mat img, bool saving) {
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in cannyFilter." << std::endl;
        return image;
    }

    try {
        if (image.type() != 0) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "ERROR: Input image to cannyFilter must be of type 8UC1." << std::endl;
        std::cout << "ERROR: Provided image was of type " << type2str(t) << "." << std::endl;
        cv::Mat empty;
        return empty;
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

    // cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3));
    // cv::dilate(edges, edges, element, cv::Point(-1, -1), 2);

//    dst = cv::Scalar::all(0);
//    image.copyTo(dst, edges);

    if (saving) {
        save(edges, path, "-canny-thick");
    }
    return edges;
}

// int main(int argc, char** argv) {
//     if (argc < 2) {
//          std::cerr << "Must pass in image to run DoG on." << std::endl;
//     } else {
//         for (int i = 1; i < argc; i++) {
//             cv::Mat image;
//             cannyFilter(argv[i], image, true);
//         }
//     }
// }