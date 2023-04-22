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
        cv::cvtColor(img, img, cv::COLOR_RGBA2GRAY);
    } else if (img.channels() == 3) {
        cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);
    }
    cv::threshold(img, img, 200, 255, cv::THRESH_BINARY);
    return img;
}

cv::Mat getCurly(cv::Mat img) {
    cv::Mat gs = grayscale(img);
    cv::imshow("gs", gs);
    cv::waitKey(0);
    std::cout << type2str(gs.type()) << std::endl;
    for (int i = 0; i < gs.rows; i++) {
        for (int j = 0; j < gs.cols; j++) {
            if (gs.at<uchar>(i, j) < 50) {
                gs.at<uchar>(i, j) = 255;
            }
        }
    }

    cv::imshow("binary", gs);
    cv::waitKey(0);
    cv::threshold(gs, gs, 250, 255, cv::THRESH_BINARY);
    return gs;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            image = read(argv[i], image);
            assert(!image.empty());
            // cv::Mat mE = morphErode(image);
            // save(mE, argv[i], "-eroded");
            // cv::Mat mD = morphDilate(image);
            // save(mD, argv[i], "-dilated");
            cv::Mat bin = binary(image);
            save(bin, argv[i], "-smoothed");
            // cv::Mat mC = morphClose(image);
            // save(mC, argv[i], "-closed");
        }
    }
    return 0;
}