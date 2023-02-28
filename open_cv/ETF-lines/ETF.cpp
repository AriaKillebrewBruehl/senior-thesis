#include "ETF.hpp"

int ws(cv::Vec2i x, cv::Vec2i y, int r) {
    if (cv::norm(x - y) < r) {
        return 1;
    }
    return 0;
}

float wm(cv::Point x, cv::Point y, int eta) {
    return 1.0;
    // return 1 / 2 * (1 + tanh(eta * (gHat(y) - gHat(x))));
}

int wd(cv::Vec2i x, cv::Vec2i y) { return abs(x.dot(y)); }

int phi(cv::Point x, cv::Point y) {
    int dot = x.dot(y);
    if (dot > 0) {
        return 1;
    }
    return -1;
}

cv::Mat ETFFilter(cv::Mat tCurX, cv::Mat tCurY, cv::Mat1b gHat, int r, int eta,
                  int nbrhood) {
    // cv::Mat gNew = sobel_mag_angle("", tCur, false);
    // std::cout << "got g new" << std::endl;
    cv::Mat tNewX = cv::Mat::zeros(tCurX.size(), tCurX.type());
    cv::Mat tNewY = cv::Mat::zeros(tCurY.size(), tCurY.type());

    for (int i = 0; i < tCurX.rows; i++) {
        for (int j = 0; j < tCurX.rows; j++) {
            cv::Vec2f sum = cv::Vec2f(0, 0);
            float k;
            cv::Point x = cv::Point(i, j);
            cv::Vec2i vX =
                cv::Vec2i(tCurX.at<int32_t>(i, j), tCurY.at<int32_t>(i, j));
            // for each pixel in the neighborhood
            for (int a = 0; a < nbrhood; a++) {
                for (int b = 0; b < nbrhood; b++) {
                    cv::Point y = cv::Point(a, b);
                    cv::Vec2i vY = cv::Vec2i(tCurX.at<int32_t>(a, b),
                                             tCurY.at<int32_t>(a, b));

                    // since gHat is normalized we only need the direction
                    int p = phi(vX, vY);
                    int s = ws(x, y, r);
                    float m = wm(x, y, eta);
                    int d = wd(vX, vY);
                    sum += float(p) * vY * float(s) * m * float(d);
                    k += float(p) * float(s) * m * float(d);
                }
            }
            sum *= 1 / k;
            tNewX.at<int>(i, j) = sum[0];
            tNewX.at<int>(i, j) = sum[1];
        }
    }
    cv::Mat channels[2] = {tNewX, tNewY};
    cv::Mat merged;
    cv::merge(channels, 2, merged);
    return merged;
}

cv::Mat1b normalizedGradientMagnitude(cv::Mat m) {
    assert(m.channels() == 2);
    cv::Mat magnitude = cv::Mat(m.size(), CV_8UC1);
    std::transform(m.begin<cv::Vec2b>(), m.end<cv::Vec2b>(),
                   magnitude.begin<uchar>(), [](cv::Vec2b p) -> uchar {
                       uchar x = p[0];
                       uchar y = p[0];
                       return uchar(sqrt(x * x + y * y));
                   });

    cv::Mat1b normalized = normalizeMatrix(magnitude);
    return normalized;
}

cv::Mat1b normalizeMatrix(cv::Mat m) {
    assert(m.channels() == 1);
    cv::Mat maxCols, max, minCols, min;
    // reduce along columns
    cv::reduce(m, maxCols, 0, cv::REDUCE_MAX);
    cv::reduce(m, minCols, 0, cv::REDUCE_MIN);
    // reduce along rows
    cv::reduce(m, max, 0, cv::REDUCE_MAX);
    cv::reduce(m, min, 0, cv::REDUCE_MIN);

    uchar max_val = max.at<uchar>(0, 0);
    uchar min_val = min.at<uchar>(0, 0);

    cv::Mat normalized = cv::Mat(m.size(), m.type());
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
    cv::Mat2b g0_merged = sobel_mag_angle(path, image, false);
    // split into X and Y components
    cv::Mat g0_channels[2];
    cv::split(g0_merged, g0_channels);
    cv::Mat1b g0X = g0_channels[0];
    assert(g0X.type() == CV_8UC1);
    cv::Mat1b g0Y = g0_channels[1];
    assert(g0Y.type() == CV_8UC1);

    // normalize by reducing values to [0, 1]
    cv::Mat1b g0X_normalized = normalizeMatrix(g0X);
    cv::Mat1b g0Y_normalized = normalizeMatrix(g0Y);

    // initial t0 matrix is the perpendicular (cc) vectors from the initial
    // gradient map g0
    cv::Mat1b t0X = g0Y_normalized * -1;
    cv::Mat1b t0Y = g0X_normalized;

    // compute normalized gradient magnitude of g0
    cv::Mat1b gHat = normalizedGradientMagnitude(g0_merged);
    std::cout << "called nGM" << std::endl;
    cv::Mat tCurX = t0X;
    cv::Mat tCurY = t0Y;
    cv::Mat tNew;
    for (int i = 0; i < 2; i++) {
        tNew = ETFFilter(tCurX, tCurY, gHat, 3, 1, 5);
        std::cout << "called ETFF" << std::endl;
        cv::Mat t_channels[2];
        cv::split(tNew, t_channels);
        cv::Mat tCurX = t_channels[0];
        cv::Mat tCurY = t_channels[1];
    }

    if (saving) {
        save(tCurX, path, "-ETF-X");
        save(tCurX, path, "-ETF-Y");
    }

    return tNew;
}