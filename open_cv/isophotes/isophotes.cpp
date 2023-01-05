#include "isophotes.hpp"

inline uchar reduceVal(const uchar val)
{
    if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    return 255;
}

std::unordered_map<uchar, int> colors{};
std::priority_queue<color_freq, std::vector<color_freq>, comp> heap;

// return a grayscale version of the image with only the L component
// TODO: change this to std::transform?
cv::Mat processColors(cv::Mat& img) {
    cv::Mat gs = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            uchar L = reduceVal(img.at<cv::Vec3b>(i, j)[0]);
            gs.at<uchar>(i,j) = (L);
            colors[L]++;
        }
    }
    return gs;
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
    
    // luminance quantization and create color frequency map
    cv::Mat processed = processColors(src);

    // generate heap
    for (std::pair<uchar, int> i : colors) {
        heap.push(i);
    }
    // take the top 25% of colors
    int s = (heap.size() <= 3) ? 1 : std::ceil(float(heap.size()) / 3.0);
   
    uchar t;
    for (int i = 0; i < s; i++) {
        t = heap.top().first;
        heap.pop();
    }
    
    // set all pixels >= t to black
    for (int i = 0; i < processed.rows; i++) {
        for (int j = 0; j < processed.cols; j++) {
            if (processed.at<uchar>(i, j) >= t) {
                processed.at<uchar>(i, j) = 0;
            } else {
               processed.at<uchar>(i, j) = 255; 
            }
        }
    }

    if (saving) {
        save(processed, path, "-isos");
    }
    
    return processed;
}
