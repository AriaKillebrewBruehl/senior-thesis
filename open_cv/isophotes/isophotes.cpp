#include "isophotes.hpp"

inline uchar reduceVal(const uchar val)
{
    if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    return 255;
}

void processColors(cv::Mat& img) {
    uchar* pixelPtr = img.data;
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            const int pi = i*img.cols*3 + j*3;
            pixelPtr[pi + 0] = reduceVal(pixelPtr[pi + 0]); // L
            // pixelPtr[pi + 1] = reduceVal(pixelPtr[pi + 1]); // a
            // pixelPtr[pi + 2] = reduceVal(pixelPtr[pi + 2]); // b
        }
    }
}  
cv::Mat getIsophotes(std::string path, cv::Mat img, bool saving) {
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
    int MAX_KERNEL_LENGTH = 15;
    cv::Mat src = image;
    // bilateral filter 
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 ) {
        cv::Mat dest;
        cv::bilateralFilter (src, dest, i, i*2, i/2 );
        src = dest;
    }

    // convert image to CIE L*a*b
    cv::cvtColor(src, src, cv::COLOR_RGB2Lab);

    // luminance quantization 
    processColors(src);

    // convert back to RGB
    cv::cvtColor(src, src, cv::COLOR_Lab2LRGB);

    if (saving) {
        save(src, path, "-isos");
    }
    
    return src;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            getIsophotes(argv[i],image, true);
        }
    }
}