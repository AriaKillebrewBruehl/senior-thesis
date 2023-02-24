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

cv::Mat ETFFilter(cv::Mat tCur, int r, int u, int k) {
    cv::Mat gNew = sobel_mag_angle("", tCur, false);
    cv::Mat tNew = cv::Mat::zeros(tCur.size(), tCur.type());
    for (int i = 0; i < tCur.rows; i++) {
        for (int j = 0; j < tCur.rows; j++) {
            float sum;
            // for each pixel in the neighborhood
            // since gHat is normalized we only need the direction
            uchar gHat = gNew.at<cv::Vec2b>(i, j)[1];
            int p = phi(tCur, cv::Point(i, j), cv::Point(i, j));
            int s = ws(cv::Point(i, j), cv::Point(i, j), r);
            float m = wm(cv::Point(i, j), cv::Point(i, j), u);
            int d = wd(tCur, cv::Point(i, j), cv::Point(i, j));
            sum += p * tCur.at<int>(i, j) * s * m * d;
            sum *= 1 / k;
            tNew.at<int>(i, j) = sum;
        }
    }
    return tNew;
}

cv::Vec2b perpendicular_normalize(cv::Vec2b g) {
    cv::Vec2b t;
    t[0] = g[0] + M_PI_2;
    t[1] = 1;
    return t;
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
    // g0<cv::Vec2b>.at(i,j)[0] gives magnitude of the vector at (i, j)
    // g0<cv::Vec2b>.at(i,j)[1] gives direction of the vector at (i, j)
    cv::Mat g0 = sobel_mag_angle(path, image, false);

    // calculate t0 by taking perpendicular vectors (CC) from g0
    cv::Mat t0 = cv::Mat(g0.size(), g0.type());
    std::transform(g0.begin<cv::Vec2b>(), g0.end<cv::Vec2b>(),
                   t0.begin<cv::Vec2b>(), perpendicular_normalize);

    return image;
}