#include "threshold-edges.hpp"

void isolate (cv::Mat& comp, int label) {
    for (int i = 0; i < comp.rows; i++) {
        for (int j = 0; j < comp.cols; j++) {
            // if the pixel is not black or the color of the label set it to black
            if (comp.at<int>(cv::Point(j, i)) != 0 && comp.at<int>(cv::Point(j, i)) != label) {
                comp.at<int>(cv::Point(j, i)) = 0;
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

// void removeComponent(cv::Mat* labels, int x, int y, int w, int h,int label) {
//     // set any pixels of the label color to black 
//     for (int i = y; i < y + h; i++) {
//         for (int j = x; j < x + w; j++) {
//             if ((*labels).at<uchar>(i, j) == label) {
//                 (*labels).at<uchar>(i, j) = 0;
//             }
//         }
//     }
// }

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

    // for (int i = 0; i < image.rows; i++) {
    //     for (int j = 0; j < image.cols; j++) {
    //         std::cout << int(image.at<uchar>(cv::Point(j, i)));
    //     }
    //     std::cout << std::endl;
    // }

    std::cout << std::endl;

    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =  cv::connectedComponentsWithStats(image, labels, stats, centroids); 
    cv::Mat labelscp = labels;

    for (int i = 0; i < labelscp.rows; i++) {
        for (int j = 0; j < labelscp.cols; j++) {
            std::cout << labelscp.at<int>(cv::Point(j, i));
        }
        std::cout << std::endl;
    }
    
    std::unordered_map<int, bool> remove; 
    // for each component except the background
    for(int i=1; i<stats.rows; i++) {
        std::cout << "starting component " << i << std::endl;
        int x = stats.at<int>(cv::Point(0, i));
        int y = stats.at<int>(cv::Point(1, i));
        int w = stats.at<int>(cv::Point(2, i));
        int h = stats.at<int>(cv::Point(3, i));
        // std::cout << "x: " << x << "y: " << y << "w: " << w << "h: " << h << std::endl;
      
        // extract just the component from labeled image
        cv::Mat comp = labels(cv::Range(y, y+h), cv::Range(x,x+w));
        std::cout << "nonzero of component" << cv::countNonZero(comp) << std::endl;
        // isolate component
        isolate(comp, i);

        // get component skeleton 
        comp.convertTo(comp, CV_8UC1);
        cv::threshold(comp, comp, 0, 255, cv::THRESH_BINARY);
        std::string file_type = path.substr(path.length()-4, 4);
        std::string output_file = path + "-thresh-thresh" + std::to_string(i) + file_type;
        cv::imwrite(output_file, comp);
        cv::Mat skel = skeleton("", comp);

        // check if skeleton meets threshold
        bool meets = meetsThreshold(skel, threshold);
        if (meets) {
            remove[i] = false;
        } else {
            remove[i] = true;
            // removeComponent(&labels, x, y, w, h, i);
        }
        std::cout << "set remove[" << i << "] = " << remove[i] <<  std::endl;
    } 

    for (int i = 0; i < labelscp.rows; i++) {
        for (int j = 0; j < labelscp.cols; j++) {
            int color = labelscp.at<int>(cv::Point(j, i));
            std::cout << "pixel(" << j << ", " << i << ") has color " << int(color) << ". remove: " << remove[color] << std::endl;
            // if color needs to be removed set it to black
            if (remove[color]) {
                labelscp.at<int>(cv::Point(i, j)) = 0;
            } else {
                labelscp.at<int>(cv::Point(i, j)) = 255;
            }
        }
    }
    
    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-thresh" + file_type;
    cv::imwrite(output_file, labels);

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