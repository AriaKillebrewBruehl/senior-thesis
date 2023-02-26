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

cv::Mat ETFFilter(cv::Mat tCurX, cv::Mat tCurY, int r, int u, int k) {
    cv::Mat gNew = sobel_mag_angle("", tCur, false);
    std::cout << "got g new" << std::endl;
    cv::Mat tNew = cv::Mat::zeros(tCur.size(), tCur.type());
    std::cout << "got g new" << std::endl;
    for (int i = 0; i < tCur.rows; i++) {
        for (int j = 0; j < tCur.rows; j++) {
            float sum;
            // for each pixel in the neighborhood
            for (int y = 0; y < k; y++) {
                for (int x = 0; x < k; x++) {
                    // since gHat is normalized we only need the direction
                    uchar gHat = gNew.at<cv::Vec2b>(i, j)[1];
                    int p = phi(tCur, cv::Point(i, j), cv::Point(y, x));
                    std::cout << "got p" << std::endl;
                    int s = ws(cv::Point(i, j), cv::Point(y, x), r);
                    std::cout << "got s" << std::endl;
                    float m = wm(cv::Point(i, j), cv::Point(y, x), u);
                    std::cout << "got wm" << std::endl;
                    int d = wd(tCur, cv::Point(i, j), cv::Point(y, x));
                    std::cout << "got wd" << std::endl;
                    sum += p * tCur.at<int>(y, x) * s * m * d;
                }
            }
            sum *= 1 / k;
            tNew.at<int>(i, j) = sum;
        }
    }
    return tNew;
}

cv::Mat normalizeMatrix(cv::Mat m, int direction) {
    assert(direction == 0 || direction == 1);
    cv::Mat max, min;
    cv::reduce(m, max, direction, cv::REDUCE_MAX);
    cv::reduce(m, min, direction, cv::REDUCE_MIN);
    uchar max_val = max.at<uchar>(0, 0);
    uchar min_val = min.at<uchar>(0, 0);

    cv::Mat normalized;
    std::transform(m.begin<uchar>(), m.end<uchar>(), normalized.begin<uchar>(),
                   [max_val, min_val](uchar p) -> uchar {
                       p -= min_val;
                       p /= (max_val - min_val);
                       return p;
                   });

    return normalized;
}

cv::Mat ETF(std::string path, cv::Mat img, bool saving) {
    cv ::Mat image;
    image = read(path, img);
    assert(!image.empty());
    if (image.type() != CV_8UC3) {
        if (image.channels() == 4) {
            cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
        } else if (image.channels() == 1) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
        }
        image.convertTo(image, CV_8UC3);
    }

    // initial gradient map
    cv::Mat g0_merged = sobel_mag_angle(path, image, false);
    // split into X and Y components
    cv::Mat g0_channels[2];
    cv::split(g0_merged, g0_channels);
    cv::Mat g0X = g0_channels[0];
    assert(g0X.type() == CV_8UC1);
    cv::Mat g0Y = g0_channels[1];
    assert(g0Y.type() == CV_8UC1);

    // normalize by reducing values to [0, 1]
    cv::Mat g0X_normalized = normalizeMatrix(g0X, 1);
    cv::Mat g0Y_normalized = normalizeMatrix(g0Y, 0);

    // initial t matrix is g0 rotated 90 degrees counter clockwise
    cv::Mat t0X = g0Y_normalized * -1;
    cv::Mat t0Y = g0X_normalized;

    /*
    // calculate t0 by taking perpendicular vectors (CC) from g0

    cv::Mat tCur = t0;
    for (int i = 0; i < 2; i++) {
        cv::Mat tNew = ETFFilter(tCur, 3, 1, 5);
        std::cout << "here" << std::endl;
        tCur = tNew;
    }

    cv::Mat visual = cv::Mat(tCur.size(), CV_8UC1);
    for (int i = 0; i < tCur.rows; i++) {
        for (int j = 0; j < tCur.cols; j++) {
            visual.at<uchar>(i, j) = tCur.at<cv::Vec2b>(i, j)[1];
        }
    }

    i
    */
    if (saving) {
        save(t0X, path, "-ETF-X");
        save(t0Y, path, "-ETF-Y");
    }

    return image;
}