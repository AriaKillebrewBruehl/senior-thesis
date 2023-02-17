#include "ETF.hpp"

int ws(cv::Point2d x, cv::Point2d y, int r) {
    if (cv::norm(x - y) < r) {
        return 1;
    }
    return 0;
}

// TO-DO: write this
float g(cv::Point) { return 1; }

float wm(cv::Point2d x, cv::Point2d y, int u) {
    return 1 / 2 * (1 + tanh(u * (g(y) - g(x))));
}

int wd(cv::Mat T, cv::Point2d x, cv::Point2d y) {
    return abs(T.at<int>(x) * T.at<int>(y));
}

int phi(cv::Mat T, cv::Point2d x, cv::Point2d y) {
    if (T.at<int>(x) * T.at<int>(y) > 0) {
        return 1;
    }
    return -1;
}

cv::Mat ETFFilter(cv::Mat Tcur, int r, int u, int k) {
    cv::Mat Tnew = cv::Mat::zeros(Tcur.size(), Tcur.type());
    for (int i = 0; i < Tcur.rows; i++) {
        for (int j = 0; j < Tcur.rows; j++) {
            float sum;
            // for each pixel in the neighborhood
            int p = phi(Tcur, cv::Point(i, j), cv::Point(i, j));
            int s = ws(cv::Point(i, j), cv::Point(i, j), r);
            float m = wm(cv::Point(i, j), cv::Point(i, j), u);
            int d = wd(Tcur, cv::Point(i, j), cv::Point(i, j));
            sum += p * Tcur.at<int>(i, j) * s * m * d;
            sum *= 1 / k;
            Tnew.at<int>(i, j) = sum;
        }
    }
    return Tnew;
}

cv::Mat ETF(std::string path, cv::Mat img, bool saving) {
    cv ::Mat image;
    image = read(path, img);
    assert(!image.empty());
    if (image.type() != 0) {
        if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2GRAY);
        } else if (image.channels() == 3) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }
        image.convertTo(image, 0);
    }

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.rows; j++) {
            float sum_i;
            float sum_j;
            // for each pixel in the neighborhood
        }
    }
    if (saving) {
        save(image, path, "-ETF");
    }
    return image;
}