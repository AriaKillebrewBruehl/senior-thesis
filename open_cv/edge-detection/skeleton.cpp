#include "skeleton.hpp"

cv::Mat skeleton(std::string path, cv::Mat img, bool saving) {
    // read in image
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in skeleton." << std::endl;
        return image;
    }

    try {
        if (image.type() != 0) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "ERROR: Input image to skeleton must be of type 8UC1." << std::endl;
        std::cout << "ERROR: Provided image was of type " << type2str(t) << "." << std::endl;
        cv::Mat empty;
        return empty;
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
