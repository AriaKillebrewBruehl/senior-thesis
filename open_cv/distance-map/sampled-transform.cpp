#include "sampled-transform.hpp"

cv::Mat DTOneDim(cv::Mat arr, std::function<int(int)> f) {
    try {
        if (arr.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Empty matrix in DTOneDim." << std::endl;
        return arr;
    }
    try {
        if (arr.rows != 1 && arr.cols != 1) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Input matrix in DTOneDim has dimensions " << arr.rows << " x " << arr.cols << ". Must be a single row or single column matrix." << std::endl;
        return arr;
    }

    cv::Mat Df = cv::Mat::zeros(arr.rows, arr.cols, CV_8UC1); // output matrix 
    int k = 0; // index of right-most parabola in lower envelope 
    std::vector<int, int> v;
    std::vector<int, int> z;

    v[0] = 0;       // locations of parabolas in lower envelope 
    z[0] = INT_MIN; // locations of boundaries between parabolas 
    z[1] = INT_MAX;

    for (int i = 0; i < arr.rows; i++) {
        for (int j = 0; j < arr.cols; j++) {
            int q = int(arr.at<uchar>(i, j));
            double s = ((f(q) + std::pow(q,2)) - (f(v[k]) + std::pow(v[k], 2))) / (2 * q - 2 * v[k]);
            if (s <= z[k]) {
                k--;
                continue;
            } else {
                k++;
                v[k] = q;
                z[k] = s;
                z[k+1] = INT_MAX;
            }
        }
    }

    k = 0;
    for (int i = 0; i < arr.rows; i++) {
        for (int j = 0; j < arr.cols; j++) {
            int q = int(arr.at<uchar>(i, j));
            while (z[k+1] < q) {
                k++;
            }
            Df.at<uchar>(i, j) = uchar((q-v[k]) * 2 + f(v[k]));
        }
    }
    return Df;
}