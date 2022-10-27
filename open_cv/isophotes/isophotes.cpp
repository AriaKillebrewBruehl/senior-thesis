#include "isophotes.hpp"

inline uchar reduceVal(const uchar val)
{
    if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    return 255;
}

void processColors(cv::Mat& img)
{
    uchar* pixelPtr = img.data;
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            const int pi = i*img.cols*3 + j*3;
            pixelPtr[pi + 0] = reduceVal(pixelPtr[pi + 0]); // B
            pixelPtr[pi + 1] = reduceVal(pixelPtr[pi + 1]); // G
            pixelPtr[pi + 2] = reduceVal(pixelPtr[pi + 2]); // R
        }
    }
}

cv::Mat getIsophotes(std::string path, cv::Mat img) {
    // read in image
    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read in image unchanged
        image = cv::imread(path);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }
        // std::cout << type2str(image.type()) << std::endl;
        // if (image.type() != 0) {
        //     throw "Image must be of type 0 (8UC1)";
        //     return image;
        // }
    } else if (!img.empty()) {
        image = img;
        // if (image.type() != 0) {
        //     throw "Image must be of type 0 (8UC1)";
        //     return image;
        // }
    }

    // convert image to CIE L*a*b
    std::cout << type2str(image.type()) << std::endl;
    std::cout << image.channels() << std::endl; 
    std::cout << image.depth() << std::endl;
    // cv::Mat cie(image.size(), image.type());
    cv::cvtColor(image, image, cv::COLOR_BGR2Lab);

    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-cie" + file_type;
    cv::imwrite(output_file, image);

    processColors(image);


    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    output_file = path + "-reduced" + file_type;
    cv::imwrite(output_file, image);
    return image;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            getIsophotes(argv[i],image);
        }
    }
}