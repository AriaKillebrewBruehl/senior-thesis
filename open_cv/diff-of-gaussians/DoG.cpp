# include "DoG.hpp"

cv::Mat DoG(std::string path, cv::Mat img) {
    // read in image
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read image
        image = cv::imread(path, 1);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }
    } else if (!img.empty()) {
        image = img;
    } 

    cv::Mat low_sigma;
    cv::Mat high_sigma;

    cv::GaussianBlur(image, low_sigma, cv::Size(3, 3), 0, 0);
    cv::GaussianBlur(image, high_sigma, cv::Size(9, 9), 0, 0);
   
    cv::Mat DoG = low_sigma - high_sigma;
    // convert to bi-level image
    cv::Mat image_th;

    double thresh = cv::threshold(DoG, image_th, 10, 255, cv::THRESH_BINARY);

    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-DoG" + file_type;
    cv::imwrite(output_file, image_th);

    return image_th;
}


// int main(int argc, char** argv) {
//     if (argc < 2) {
//          std::cerr << "Must pass in image to run DoG on." << std::endl;
//     } else {
//         for (int i = 1; i < argc; i++) {
//             cv::Mat image;
//             image = cv::imread(argv[i], 1);
//             DoG("", image);
//         }
//     }
// }