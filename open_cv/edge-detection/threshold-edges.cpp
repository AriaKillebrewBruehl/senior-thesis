#include "threshold-edges.hpp"

cv::Mat isolate(cv::Mat& comp, int label) {
    cv::Mat isolated;
    comp.copyTo(isolated);
    for (int i = 0; i < comp.rows; i++) {
        for (int j = 0; j < comp.cols; j++) {
            // if the pixel is not black or the color of the label set it to
            // black
            if (comp.at<int>(cv::Point(j, i)) != 0 &&
                comp.at<int>(cv::Point(j, i)) != label) {
                isolated.at<int>(cv::Point(j, i)) = 0;
            }
        }
    }
    return isolated;
}
bool meetsThreshold(cv::Mat img, int threshold) {
    if (img.empty()) {
        return false;
    }
    if (cv::countNonZero(img) < threshold) {
        return false;
    }
    return true;
}

cv::Mat threshold(std::string path, cv::Mat img, int threshold, bool saving) {
    // read in image and convert to grayscale
    cv::Mat image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in threshold."
                  << std::endl;
        return image;
    }

    try {
        if (image.type() != 0) {
            throw image.type();
        }
    } catch (int t) {
        std::cout << "ERROR: Input image to threshold must be of type 8UC1."
                  << std::endl;
        std::cout << "ERROR: Provided image was of type " << type2str(t) << "."
                  << std::endl;
        cv::Mat empty;
        return empty;
    }

    // convert to binary
    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);

    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =
        cv::connectedComponentsWithStats(image, labels, stats, centroids);

    std::unordered_map<int, bool> remove;
    // for each component except the background
    for (int i = 1; i < stats.rows; i++) {
        int x = stats.at<int>(cv::Point(0, i));
        int y = stats.at<int>(cv::Point(1, i));
        int w = stats.at<int>(cv::Point(2, i));
        int h = stats.at<int>(cv::Point(3, i));

        // extract just the component from labeled image
        cv::Mat comp = labels(cv::Range(y, y + h), cv::Range(x, x + w));
        // isolate component
        cv::Mat isolated = isolate(comp, i);

        // get component skeleton
        isolated.convertTo(isolated, CV_8UC1);
        cv::threshold(isolated, isolated, 0, 255, cv::THRESH_BINARY);
        cv::Mat skel = skeleton("", isolated, false);

        remove[i] = !(meetsThreshold(skel, threshold));
    }

    for (int i = 0; i < labels.rows; i++) {
        for (int j = 0; j < labels.cols; j++) {
            int color = labels.at<int>(cv::Point(j, i));
            // avoid already black pixels
            if (color == 0) {
                continue;
            }
            // if color needs to be removed set it to black
            if (remove[color]) {
                labels.at<int>(cv::Point(j, i)) = 0;
            } else {
                labels.at<int>(cv::Point(j, i)) = 255;
            }
        }
    }

    if (saving) {
        save(labels, path, "-thresh");
    }

    return labels;
}