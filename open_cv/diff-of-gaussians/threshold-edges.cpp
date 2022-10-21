#include "threshold-edges.hpp"

cv::Mat threshold(std::string path, cv::Mat img) {
    // read in image
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read image
        image = cv::imread(path, 0);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }   
    } else if (!img.empty()) {
        image = img;
    }
    // convert to binary 
    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);

    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;

    int numComps =  cv::connectedComponentsWithStats(image, labels, stats, centroids); 	

    
    //std::cout << labels << std::endl;
    std::cout << "stats.size()=" << stats.size() << std::endl;
    //std::cout << centroids << std::endl;
    
    for(int i=0; i<stats.rows; i++) {
        int x = stats.at<int>(cv::Point(0, i));
        int y = stats.at<int>(cv::Point(1, i));
        int w = stats.at<int>(cv::Point(2, i));
        int h = stats.at<int>(cv::Point(3, i));
      
        // extract just the component
        std::cout << "x=" << x << " y=" << y << " w=" << w << " h=" << h << std::endl;
        cv::Mat comp = image(cv::Range(y, y+h), cv::Range(x,x+w));
      
        // save image
        
       
        if (path == "") {
            srand (time(NULL));
             int rand = std::rand() % 1000;
            path = "../images/" + std::to_string(rand) + ".png";
        }
        std::string file_type = path.substr(path.length()-4, 4);
        std::string output_file = path + "-thresh-" + std::to_string(i) + file_type;
        cv::imwrite(output_file, comp);
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
            // image = cv::imread(argv[i], 0);
            threshold(argv[i], image);
        }
    }
}