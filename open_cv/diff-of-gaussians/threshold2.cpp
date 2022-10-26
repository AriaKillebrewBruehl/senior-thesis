#include "threshold-edges.hpp"

cv::Mat isolate (cv::Mat& comp, int label) {
    cv::Mat isolated;
    comp.copyTo(isolated);
    for (int i = 0; i < comp.rows; i++) {
        for (int j = 0; j < comp.cols; j++) {
            // if the pixel is not black or the color of the label set it to black
            if (comp.at<int>(cv::Point(j, i)) != 0 && comp.at<int>(cv::Point(j, i)) != label) {
                isolated.at<int>(cv::Point(j, i)) = 0;
            }
        }
    }
    return isolated;
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
    
    std::unordered_map<int, bool> remove; 
    // for each component except the background
    for(int i=1; i<stats.rows; i++) {
        std::cout << "stating component " << i << std::endl;
        int x = stats.at<int>(cv::Point(0, i));
        int y = stats.at<int>(cv::Point(1, i));
        int w = stats.at<int>(cv::Point(2, i));
        int h = stats.at<int>(cv::Point(3, i));
        std::cout << x << " " << y << " " << w << " " << h << std::endl;
      
        // extract just the component from labeled image
        cv::Mat comp = labels(cv::Range(y, y+h), cv::Range(x,x+w));
        std::cout << cv::countNonZero(comp) << std::endl;
        // isolate component
        cv::Mat isolated = isolate(comp, i);
        // get component skeleton 
        comp.convertTo(isolated, CV_8UC1);
        cv::threshold(isolated, isolated, 0, 255, cv::THRESH_BINARY);
        std::cout << cv::countNonZero(isolated) << std::endl;
        std::string file_type = path.substr(path.length()-4, 4);
        std::string output_file = path + "-threshcomp" + std::to_string(i) + file_type;
        cv::imwrite(output_file, isolated);
        cv::Mat skel = skeleton("", isolated);

        // check if skeleton meets threshold
        remove[i] = !(meetsThreshold(skel, threshold));
        // bool meets = meetsThreshold(skel, threshold);
        // if (meets) {
        //     remove[i] = false;
        // } else {
        //     remove[i] = true;
        // }
    } 

    for (int i = 0; i < labels.rows; i++) {
        for (int j = 0; j < labels.cols; j++) {
            int color = labels.at<int>(cv::Point(j, i));
            // avoid already black pixels
            if (color == 0) {
                continue;
            }
            // if color needs to be removed set it to black
            if (remove[color]) {
                labels.at<int>(cv::Point(j, i)) = 0;
            } else {
                labels.at<int>(cv::Point(j, i)) = 255;
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
            threshold(argv[i], image, 50);
        }
    }
}