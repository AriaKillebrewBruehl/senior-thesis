#include "ETF.hpp"

int ws(cv::Vec2i x, cv::Vec2i y, int r) {
    if (cv::norm(x - y) < r) {
        return 1;
    }
    return 0;
}

float wm(cv::Point x, cv::Point y, int eta) {
    return 1 / 2 * (1 + tanh(eta * (gHat(y) - gHat(x))));
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
    assert(tCurX.type() == CV_32FC1);
    assert(tCurY.type() == CV_32FC1);
    cv::Mat tNewX = cv::Mat::zeros(tCurX.size(), tCurX.type());
    cv::Mat tNewY = cv::Mat::zeros(tCurY.size(), tCurY.type());

    for (int i = 0; i < tCurX.rows; i++) {
        for (int j = 0; j < tCurX.rows; j++) {
            cv::Vec2f sum = cv::Vec2f(0, 0);
            float k;
            cv::Point x = cv::Point(i, j);
            cv::Vec2i vX =
                cv::Vec2i(tCurX.at<float32_t>(x), tCurY.at<float32_t>(x));
            // for each pixel in the neighborhood
            for (int a = 0; a < nbrhood; a++) {
                for (int b = 0; b < nbrhood; b++) {
                    cv::Point y = cv::Point(a, b);
                    cv::Vec2i vY = cv::Vec2i(tCurX.at<float32_t>(y),
                                             tCurY.at<float32_t>(y));

                    int p = phi(vX, vY);
                    int s = ws(x, y, r);
                    float m = wm(x, y, eta);
                    int d = wd(vX, vY);
                    sum += float(p) * vY * float(s) * m * float(d);
                    k += float(p) * float(s) * m * float(d);
                }
            }
            sum *= 1 / k;
            tNewX.at<uchar>(x) = sum[0];
            tNewX.at<uchar>(x) = sum[1];
        }
    }
    cv::Mat channels[2] = {tNewX, tNewY};
    cv::Mat merged;
    cv::merge(channels, 2, merged);
    return merged;
}

cv::Mat normalizedGradientMagnitude(cv::Mat m) {
    assert(m.channels() == 2);
    cv::Mat magnitude = cv::Mat(m.size(), CV_32FC1);
    std::transform(m.begin<cv::Vec2b>(), m.end<cv::Vec2b>(),
                   magnitude.begin<float32_t>(), [](cv::Vec2b p) -> float32_t {
                       uchar x = p[0];
                       uchar y = p[0];
                       return float32_t(sqrt(x * x + y * y));
                   });

    cv::Mat1b normalized = normalizeMatrix(magnitude);
    return normalized;
}

cv::Mat normalizeMatrix(cv::Mat m) {
    assert(m.channels() == 1);
    std::cout << "m type: " << type2str(m.type()) << std::endl;
    cv::Mat maxCols, max, minCols, min;
    std::cout << "maxCols type: " << type2str(maxCols.type()) << std::endl;
    // // reduce along columns
    // cv::reduce(m, maxCols, 0, cv::REDUCE_MAX);
    // cv::reduce(m, minCols, 0, cv::REDUCE_MIN);
    // // reduce along rows
    // cv::reduce(m, max, 0, cv::REDUCE_MAX);
    // cv::reduce(m, min, 0, cv::REDUCE_MIN);

    // uchar max_val = max.at<uchar>(0, 0);
    // uchar min_val = min.at<uchar>(0, 0);

    cv::Mat normalized = cv::Mat(m.size(), CV_32FC1);
    std::transform(m.begin<uchar>(), m.end<uchar>(),
                   normalized.begin<float32_t>(),
                   [/*max_val, min_val*/](uchar p) -> float32_t {
                       // p -= min_val;
                       // float32_t n = float32_t(p) / float32_t(max_val -
                       // min_val);
                       float32_t n = float32_t(p) / 255.0;
                       return n;
                   });

    std::cout << type2str(normalized.type()) << std::endl;
    return normalized;
}

cv::Mat scaleUpMatrix(cv::Mat m) {
    assert(m.channels() == 1);
    cv::Mat scaledUp = cv::Mat(m.size(), CV_8UC1);
    std::transform(m.begin<schar>(), m.end<schar>(), scaledUp.begin<uchar>(),
                   [](schar p) -> uchar { return abs(p) * 255; });

    return scaledUp;
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
    // save(g0X, path, "-g0X");
    std::cout << "intitial g0X: \n" << g0X << std::endl;
    cv::Mat1b g0Y = g0_channels[1];
    // save(g0Y, path, "-g0Y");
    std::cout << "intitial g0Y: \n" << g0Y << std::endl;

    // normalize by reducing values to [0, 1]
    cv::Mat g0X_normalized = normalizeMatrix(g0X);
    std::cout << type2str(g0X_normalized.type()) << std::endl;
    assert(g0X_normalized.type() == CV_32FC1);
    std::cout << "normalized g0X: \n" << g0X_normalized << std::endl;
    cv::Mat g0Y_normalized = normalizeMatrix(g0Y);
    assert(g0Y_normalized.type() == CV_32FC1);
    std::cout << "normalized g0Y: \n" << g0Y_normalized << std::endl;

    cv::Mat g0X_scaled = scaleUpMatrix(g0X_normalized);
    std::cout << "rescaled g0X: \n" << g0Y << std::endl;
    cv::Mat g0Y_scaled = scaleUpMatrix(g0Y_normalized);
    std::cout << "rescaled g0Y: \n" << g0Y << std::endl;

    // initial t0 matrix is the perpendicular (cc) vectors from the initial
    // gradient map g0
    cv::Mat t0X = g0Y_normalized * -1;
    assert(t0X.type() == CV_32FC1);
    std::cout << "t0X: " << t0X << std::endl;
    cv::Mat t0Y = g0X_normalized;
    std::cout << "t0Y: " << t0Y << std::endl;
    assert(t0Y.type() == CV_32FC1);
    // std::string tagX = "-ETF-X-" + std::to_string(10);
    // std::string tagY = "-ETF-Y-" + std::to_string(10);
    // cv::Mat tCurXSacled = scaleUpMatrix(t0X);
    // cv::Mat tCurYSacled = scaleUpMatrix(t0Y);
    // save(tCurXSacled, path, tagX);
    // save(tCurXSacled, path, tagY);

    // compute normalized gradient magnitude of g0
    cv::Mat gHat = normalizedGradientMagnitude(g0_merged);
    assert(gHat.type() == CV_8UC1);
    cv::Mat tCurX = t0X;
    cv::Mat tCurY = t0Y;

    cv::Mat tNew;
    for (int i = 0; i < 2; i++) {
        std::string tagX = "-ETF-X-" + std::to_string(i + 1);
        std::string tagY = "-ETF-Y-" + std::to_string(i + 1);
        cv::Mat tCurXSacled = scaleUpMatrix(tCurX);
        cv::Mat tCurYSacled = scaleUpMatrix(tCurY);
        save(tCurXSacled, path, tagX);
        save(tCurXSacled, path, tagY);
        tNew = ETFFilter(tCurX, tCurY, gHat, 3, 1, 5);
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