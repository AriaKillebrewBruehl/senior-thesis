#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/invert.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "sobel_operator.hpp"

int ws(cv::Vec2i x, cv::Vec2i y, int r);
float g(cv::Point);
float wm(cv::Point x, cv::Point y, int eta);
int wd(cv::Vec2i x, cv::Vec2i y);
int phi(cv::Point x, cv::Point y);
cv::Mat1b normalizedGradientMagnitude(cv::Mat m);
cv::Mat1b normalizeMatrix(cv::Mat m);
cv::Mat ETFFilter(cv::Mat tCurX, cv::Mat tCurY, cv::Mat1b gHat, int r, int eta,
                  int nbrhood);
cv::Mat ETF(std::string path, cv::Mat img, bool saving)

    /*
     cv::Mat normalizeMatrix(cv::Mat m, int direction)
     Normalize a matrix representing the sobel operator
     applied in either X or Y direction. Output matrix
     will have values in the range [0, 1]. If m is
     the sobel operator applied in the X direction use
     `direction = 1`, use `directon = 0` for sobel operator
     applied in the Y direciton.
     */
    cv::Mat normalizeMatrix(cv::Mat m, int direction);
cv::Mat ETF(std::string path, cv::Mat img, bool saving);