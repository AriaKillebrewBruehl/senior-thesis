#include "isophotes.hpp"

inline uchar reduceVal(const uchar val)
{
    if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    return 255;
}

std::unordered_map<int, int> colors{};
std::priority_queue<color_freq, std::vector<color_freq>, comp> heap;

// reduce values on L and remove a and b 
void processColors(cv::Mat& img) {
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int L = reduceVal(img.at<cv::Vec3b>(i, j)[0]);
            img.at<cv::Vec3b>(i, j)[0] =  L;
            img.at<cv::Vec3b>(i, j)[1] = 0; // a
            img.at<cv::Vec3b>(i, j)[2] = 0; // b
            colors[L]++;
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
    
    // luminance quantization and create color frequency map
    processColors(src);

    // generate heap
    for (std::pair<int, int> i : colors) {
        heap.push(i);
    }
    std::cout << "made heap" << std::endl;
    // take the top 25% of colors
    int s = (heap.size() <= 4) ? 1 : heap.size() / 4;
    std::cout << "s: " << s << std::endl;
    int t;

    std::priority_queue<color_freq, std::vector<color_freq>, comp> heap2 = heap;
    while (!heap2.empty()) {
        std::cout << heap2.top().first << " " << heap2.top().second << std::endl;
        heap2.pop();
    }
    for (int i = 0; i < s; i++) {
        t = heap.top().first;
        heap.pop();
    }
    std::cout << "t: " << t <<  std::endl;
    cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);
    save(src, path, "recolored");
    cv::threshold(src, src, t, 255, cv::THRESH_BINARY);
    save(src, path, "-isos");
    if (saving) {
        save(src, path, "-isos");
    }
    
    return src;
}
