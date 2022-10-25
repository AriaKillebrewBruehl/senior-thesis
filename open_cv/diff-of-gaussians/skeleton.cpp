#include "skeleton.hpp"

cv::Mat skeleton(std::string path, cv::Mat img) {
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
        if (image.type() != 0) {
            throw "Image must be of type 0 (8UC1)";
            return image;
        }
    } else if (!img.empty()) {
        image = img;
        if (image.type() != 0) {
            throw "Image must be of type 0 (8UC1)";
            return image;
        }
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


    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-skel" + file_type;
    cv::imwrite(output_file, skel);

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