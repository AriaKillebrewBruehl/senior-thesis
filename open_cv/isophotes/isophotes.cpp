#include "isophotes.hpp"

cv::Mat getIsophotes(std::string path, cv::Mat img, int thresh, bool saving) {
    // read in image
    cv::Mat image = read(path, img);
    assert(!image.empty());

    if (image.type() != 16) {
        if (image.channels() == 1) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
        }
        if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
        }
        image.convertTo(image, 16);
    }

    int MAX_KERNEL_LENGTH = 15;
    cv::Mat src = image;
    // bilateral filter
    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2) {
        cv::Mat dest;
        cv::bilateralFilter(src, dest, i, i * 2, i / 2);
        src = dest;
    }
    // convert image to CIE L*a*b
    cv::cvtColor(src, src, cv::COLOR_RGB2Lab);

    // luminance quantization and create color frequency map
    cv::Mat processed = processColors(src);

    std::unordered_map<uchar, int> colors{};
    std::priority_queue<color_freq, std::vector<color_freq>, comp> heap;
    for (int i = 0; i < processed.rows; i++) {
        for (int j = 0; j < processed.cols; j++) {
            uchar c = processed.at<uchar>(i, j);
            colors[c]++;
        }
    }

    // generate heap
    for (std::pair<uchar, int> i : colors) {
        heap.push(i);
    }

    if (thresh > heap.size()) {
        thresh = heap.size();
    }
    uchar t;
    for (int i = 0; i < thresh; i++) {
        t = heap.top().first;
        heap.pop();
    }

    // set all pixels >= t (lighter than t) to white
    for (int i = 0; i < processed.rows; i++) {
        for (int j = 0; j < processed.cols; j++) {
            processed.at<uchar>(i, j) =
                processed.at<uchar>(i, j) >= t ? uchar(255) : uchar(0);
        }
    }

    if (saving) {
        save(processed, path, "-highlights");
    }

    return processed;
}