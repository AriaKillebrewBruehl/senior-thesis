#include "isophotes.hpp"

inline uchar reduceVal(const uchar val)
{
    if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    return 255;
}

// reduce values on L and remove a and b 
void processColors(cv::Mat& img) {
    uchar* pixelPtr = img.data;
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            const int pi = i*img.cols*3 + j*3;
            pixelPtr[pi + 0] = reduceVal(pixelPtr[pi + 0]); // L
            pixelPtr[pi + 1] = 0; // a
            pixelPtr[pi + 2] = 0; // b
        }
    }
} 

void onlyL(cv::Mat& img) {
    uchar* pixelPtr = img.data;
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            const int pi = i*img.cols*3 + j*3;
            pixelPtr[pi + 1] = 0; // a
            pixelPtr[pi + 2] = 0; // b
        }
    }
} 

cv::Mat getIsophotes(std::string path, cv::Mat img, bool saving) {
    // read in image
    cv::Mat image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in getIsophotes." << std::endl;
        return image;
    }

    try {
        if (image.type() != 16) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "ERROR: Input image in getIsophotes must be of type 8UC3." << std::endl;
        std::cout << "ERROR: Provided image was of type " << type2str(t) << "." << std::endl;
        cv::Mat empty;
        return empty;
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

    // convert image to 4 channels
    cv::Mat threshed(src.size(), CV_MAKE_TYPE(src.depth(), 4));
    int from_to[] = { 0,0, 1,1, 2,2, 2,3 };
    cv::mixChannels(&src,1,&threshed,1,from_to,4);
    // convert to grayscale and threshold image
    cv::cvtColor(threshed, threshed, cv::COLOR_RGB2GRAY);
    cv::threshold(threshed, threshed, 64, 255, cv::THRESH_BINARY);

    if (saving) {
        save(threshed, path, "-isos*");
    }
    
    return threshed;
}

// int main(int argc, char** argv) {
//     if (argc < 2) {
//          std::cerr << "Must pass in image to run DoG on." << std::endl;
//     } else {
//         for (int i = 1; i < argc; i++) {
//             cv::Mat image;
//             getIsophotes(argv[i],image, true);
//         }
//     }
// }