#include "extract-edges.hpp"

inline uchar reduceVal(const uchar val) {
    if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    return 255;
}

// return a grayscale version of the image with only the L component
// TODO: change this to std::transform?
cv::Mat processColors2(cv::Mat& img) {
    cv::Mat gs = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            uchar L = reduceVal(img.at<cv::Vec3b>(i, j)[0]);
            gs.at<uchar>(i, j) = (L);
        }
    }
    return gs;
}

cv::Mat extractEdges(std::string path, cv::Mat img, int thresh, bool saving) {
    // read images and resize
    cv::Mat image;
    image = read(path, img);
    cv::Mat correct;

    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in sample." << std::endl;
        return image;
    }

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
    cv::Mat processed = processColors2(src);

    assert(processed.type() == 0);

    cv::Mat edges = cannyFilter(path, processed, true);

    cv::Mat element =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 1));

    // extract edges via threshold
    cv::Mat extracted = threshold(path, edges, thresh, false);

    // // morphological operations
    cv::Mat morphed2;
    extracted.convertTo(extracted, CV_8UC1);
    cv::morphologyEx(extracted, morphed2, cv::MORPH_OPEN, element,
                     cv::Point(-1, -1), 3);

    if (saving) {
        save(morphed2, path, "-extracted-w-morph");
    }
    return extracted;
}
