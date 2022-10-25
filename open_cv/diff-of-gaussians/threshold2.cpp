#include "threshold-edges.hpp"

bool meetsThreshold(cv::Mat img, int threshold) {
    if (img.empty()) {
        return false;
    }
    if (cv::countNonZero(img) < threshold) {
        return false;
    }
    return true;
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
    
    // convert to binary and get skeleton 
    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);
    cv::Mat skel = skeleton(path, image);

    // get the components of skeleton
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =  cv::connectedComponentsWithStats(skel, labels, stats, centroids); 	
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-thresh-labels" + file_type;
    cv::imwrite(output_file, labels);
    
    for(int i=0; i<stats.rows; i++) {
        int x = stats.at<int>(cv::Point(0, i));
        int y = stats.at<int>(cv::Point(1, i));
        int w = stats.at<int>(cv::Point(2, i));
        int h = stats.at<int>(cv::Point(3, i));
      
        // extract just the component
        cv::Mat comp = skel(cv::Range(y, y+h), cv::Range(x,x+w));

        // check if component meets threshold
        bool meets = meetsThreshold(comp, threshold);
        if (meets) {
            continue;
        } else {
            // if the component does not meet the threshold, set each pixel to black 
            for (int i = y; i < y + h; i++) {
                for (int j = x; j < x + w; j++) {
                    // std::cout << int(image.at<uchar>(i, j)) << std::endl;
                    if (image.at<uchar>(i, j) == 255) {
                        // mark short lines as gray 
                        image.at<uchar>(i, j) = 0;
                    }
                }
            }
        }
    }
    
    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    file_type = path.substr(path.length()-4, 4);
    output_file = path + "-thresh" + file_type;
    cv::imwrite(output_file, image);

    return image;
}


int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            threshold(argv[i], image, 10000);
        }
    }
}