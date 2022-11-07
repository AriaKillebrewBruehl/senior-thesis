#include "skeleton.hpp"

cv::Mat skeleton(std::string path, cv::Mat img, bool saving) {
    // read in image
    cv::Mat image;
    image = read(path, img);

    if (image.type() != 0) {
        throw "Image must be single chanel grayscale.";
    }
    
    // convert to binary 
    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);

    // skeleton image and temp image 
    cv::Mat copy;
    image.copyTo(copy);
    cv::Mat skel(image.size(), CV_8UC1, cv::Scalar(0));
    cv::Mat temp(image.size(), CV_8UC1);
    cv::Mat eroded;

    // use a 3 x 3 cross shape for 4-connexity 
    //   []
    // [][][]
    //   []
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
    
    // if the image is not totally white
    if (cv::countNonZero(copy) != (copy.rows * copy.cols)) {
        bool done;	
        do
        {
            cv::erode(copy, eroded, element);
            cv::dilate(eroded, temp, element); // temp = open(image)
            cv::subtract(copy, temp, temp);
            cv::bitwise_or(skel, temp, skel);
            eroded.copyTo(copy);
            
            done = (cv::countNonZero(copy) == 0);
        } while (!done);
    } else {
        skel = copy; 
    }

    if (saving) {
        save(image, path, "-skel");
    }

    return skel;
}

// int main(int argc, char** argv) {
//     if (argc < 2) {
//          std::cerr << "Must pass in image to run DoG on." << std::endl;
//     } else {
//         for (int i = 1; i < argc; i++) {
//             cv::Mat image;
//             skeleton(argv[i],image );
//         }
//     }
// }