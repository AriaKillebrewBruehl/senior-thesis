#include "threshold-edges.hpp"

void isolate (cv::Mat& comp, int label) {
    std::cout << "isolating on: " << label << std::endl;
    for (int i = 0; i < comp.rows; i++) {
        for (int j = 0; j < comp.cols; j++) {
            std::cout << cv::Point(i, j) << std::endl;
            // if the pixel is not black or the color of the label set it to black
            if (comp.at<uchar>(cv::Point(i, j)) != 0 && comp.at<uchar>(cv::Point(i, j)) != label) {
                comp.at<uchar>(cv::Point(i, j)) = 0;
            }
        }
    }
}
bool meetsThreshold(cv::Mat img, int threshold) {
    if (img.empty()) {
        return false;
    }
    if (cv::countNonZero(img) < threshold) {
        return false;
    }
    return true;
}

void removeComponent(cv::Mat* labels, int x, int y, int w, int h,int label) {
    // set any pixels of the label color to black 
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            if ((*labels).at<uchar>(i, j) == label) {
                (*labels).at<uchar>(i, j) = 0;
            }
        }
    }
}

cv::Mat threshold(std::string path, cv::Mat img, int threshold) {
    // read in image
    // image must be of type 8UC1

    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read in image unchanged
        image = cv::imread(path, 0);
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

    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =  cv::connectedComponentsWithStats(image, labels, stats, centroids); 
    
    // for each component except the background
    for(int i=1; i<stats.rows; i++) {
        std::cout << "starting component " << i << std::endl;
        int x = stats.at<int>(cv::Point(0, i));
        int y = stats.at<int>(cv::Point(1, i));
        int w = stats.at<int>(cv::Point(2, i));
        int h = stats.at<int>(cv::Point(3, i));
      
        // extract just the component from labeled image
        cv::Mat comp = labels(cv::Range(y, y+h), cv::Range(x,x+w));
        // isolate component
        std::cout << "nonzero before isolating: " << cv::countNonZero(comp) << std::endl;
        isolate(comp, i);
        std::cout << "nonzero after isolating: "  << cv::countNonZero(comp) << std::endl;
        // get component skeleton 

        comp.convertTo(comp, CV_8UC1);
        cv::threshold(comp, comp, 1, 255, cv::THRESH_BINARY);
        std::string file_type = path.substr(path.length()-4, 4);
        std::string output_file = path + "-thresh-skel-"+ std::to_string(i) + file_type;
        cv::imwrite(output_file, comp);
        cv::Mat skel = skeleton("", comp);
        std::cout << "nonzero of skeleton: " << cv::countNonZero(skel) << std::endl;
        // check if skeleton meets threshold
        bool meets = meetsThreshold(skel, threshold);
        if (meets) {
            continue;
        } else {
            removeComponent(&labels, x, y, w, h, i);
        }
        std::cout << "what?" << std::endl;
    }  

    // threshold labeled image 
    cv::Mat test;
    labels.convertTo(test, CV_32FC1);
    cv::threshold(test, test, 0, 255, cv::THRESH_BINARY);
    
    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-thresh" + file_type;
    cv::imwrite(output_file, test);

    return labels;
}


int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            threshold(argv[i], image, 10);
        }
    }
}