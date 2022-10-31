#include <string>
#include <opencv2/opencv.hpp>

void save(cv::Mat image, std::string path, std::string suffix) {
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + suffix + file_type;
    cv::imwrite(output_file, image);

    return;
}