#include "extract-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, int thresh, bool saving) {
    // read images and resize
    cv::Mat image;
    image = read(path, img);
    cv::Mat correct;

    assert(!image.empty());

    if (image.type() != 16) {
        if (image.channels() != 3) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
        }
        image.convertTo(image, 16);
    }
    assert(image.type() == 16);

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
    cv::Mat processed = processColors(src, nullptr);

    assert(processed.type() == 0);
    cv::Mat edges = cannyFilter(path, processed, false);

    // extract edges via threshold
    cv::Mat extracted = threshold(path, edges, thresh, false);

    // cv::Mat element =
    //     cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 1));

    // // // morphological operations
    // cv::Mat morphed2;
    // if (extracted.type() != 0 || extracted.type() != 16) {
    //     if (extracted.channels() == 3) {
    //         extracted.convertTo(extracted, 16);
    //     } else {
    //         extracted.convertTo(extracted, 0);
    //     }
    // }
    // cv::morphologyEx(extracted, morphed2, cv::MORPH_OPEN, element,
    //                  cv::Point(-1, -1), 3);

    cv::Mat inverted = invert(extracted);

    if (saving) {
        save(inverted, path, "-extracted");
    }
    return inverted;
}
