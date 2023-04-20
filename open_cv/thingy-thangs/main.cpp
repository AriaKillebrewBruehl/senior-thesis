#include "main.hpp"

cv::Mat threshold(cv::Mat img) {
    double thresh = cv::threshold(img, img, 127, 255, cv::THRESH_BINARY);
    return img;
}

cv::Mat CIE(cv::Mat img) {
    cv::cvtColor(img, img, cv::COLOR_RGB2Lab);
    return img;
}

cv::Mat morphErode(cv::Mat img) {
    cv::Mat element =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));
    cv::morphologyEx(img, img, cv::MORPH_ERODE, element, cv::Point(-1, -1));
    return img;
}

cv::Mat morphDilate(cv::Mat img) {
    cv::Mat element =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));
    cv::morphologyEx(img, img, cv::MORPH_DILATE, element, cv::Point(-1, -1));
    return img;
}

cv::Mat morphOpen(cv::Mat img) {
    cv::Mat element =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(img, img, cv::MORPH_OPEN, element, cv::Point(-1, -1));
    return img;
}

cv::Mat morphClose(cv::Mat img) {
    cv::Mat element =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(img, img, cv::MORPH_CLOSE, element, cv::Point(-1, -1));
    return img;
}

cv::Mat grayscale(cv::Mat img) {
    if (img.channels() == 4) {
        cv::cvtColor(img, img, cv::COLOR_RGBA2RGB);
    }
    cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);
    return img;
}

cv::Mat binary(cv::Mat img) {
    if (img.channels() == 4) {
        cv::cvtColor(img, img, cv::COLOR_RGBA2RGB);
    } else if (img.channels() == 3) {
        cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);
    }
    cv::threshold(img, img, 10, 255, cv::THRESH_BINARY_INV);
    return img;
}

cv::Mat enlargeDots(std::string pathSeeds, cv::Mat imgSeeds, bool saving) {
    cv::Mat seeds = read(pathSeeds, imgSeeds);
    assert(!seeds.empty());

    if (seeds.type() != 0) {
        if (seeds.channels() != 1) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGB2GRAY);
        }
        seeds.convertTo(seeds, 0);
    }
    assert(seeds.type() == 0);

    int scale = 6;
    cv::Mat rendered = cv::Mat(seeds.rows * scale, seeds.cols * scale, CV_8UC1,
                               cv::Scalar(255));

    for (int i = 0; i < seeds.rows; i++) {
        for (int j = 0; j < seeds.cols; j++) {
            if (seeds.at<uchar>(i, j) == 0) {
                cv::circle(rendered, cv::Point2d(j * scale, i * scale), int(10),
                           cv::Scalar(0), -1);
            }
        }
    }

    if (saving) {
        save(rendered, pathSeeds, "-better-dots");
    }

    return rendered;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            image = read(argv[i], image);
            assert(!image.empty());
            enlargeDots(argv[i], image, true);
        }
    }
    return 0;
}