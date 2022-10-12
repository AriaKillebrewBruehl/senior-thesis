#include "extract-edges.hpp"


cv::Mat morph_open(std::string file_path, cv::Mat image) {
    cv::Mat opened;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));

    cv::morphologyEx(image, opened, cv::MORPH_OPEN, element);
        // save image
    std::string file_type = file_path.substr(file_path.length()-4, 4);
    std::string output_file_morphed = file_path + "-morphed-"+ file_type;
    cv::imwrite(output_file_morphed, opened);
    
    return opened;
}

// int connected_components(std::string file_path, cv::Mat image) {
//     cv::Mat labels;
//     cv::Mat stats;
//     cv::Mat centroids;
//     int nLabels = cv::connectedComponentsWithStats(image, labels, stats, centroids); 

//     int sizes = stats(labels, )

// }


// int remove_small_edges(std::string img) {
//     cv::Mat image;
//     // read image
//     image = cv::imread(img, 1);
//     if (img.empty()) {
//         throw "Not a valid image file.";
//         return 1;
//     }

//     cv::Mat gray(image.size(), CV_8U);
//     cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
//     // cv::Mat gray;
//     // gray = grayscale(image);

//     cv::Mat image_th;
//     cv::Mat bin_mat(gray.size(), gray.type());
//     cv::adaptiveThreshold(gray, image_th, 255,
// 			  cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 3, 5);


//     cv::Mat dest;
//     cv::Mat stats;
//     cv::Mat centroids;

//     int comp = cv::connectedComponentsWithStats(image, dest, stats, centroids);

//     std::cout << dest << std::endl;
//     std::cout << stats << std::endl;
//     std::cout << centroids << std::endl;

//     return 0;
    
// }

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat img = DoG(argv[i]);
            // save image
            std::string path = std::to_string(*argv[i]);
           //  std::string file_type = path.substr(path.length()-4, 4);
            std::string DoG_path = path + "-DoG.png";
            morph_open(DoG_path, img);
        }
    }
}