#include "sampled-transform.hpp"

// indicator function for membership in a set of seed pixels
int f(int p) {
    // std::cout << "f(" << p << ") = ";
    if (p == 255) {
        // std::cout << "255" << std::endl;
        return 0;
    }
    // std::cout << "INT_MAX" << std::endl;

    return INT_MAX;
}

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
    std::vector<int> v{0};
    std::vector<int> z{INT_MIN, INT_MAX}; // locations of boundaries between parabolas 

    for (int i = 0; i < arr.rows; i++) {
        for (int j = 0; j < arr.cols; j++) {
            std::cout << k << std::endl;
            int q = arr.at<int>(i, j);
            // std::cout << "(" << i << ", " << j << ") = " << q << ", f(q) = " << f(q) << std::endl;
            
            bool done = false;
            int s;
            while (!done) {
                int r = arr.at<int>(0, v[k]);
                s = ((f(q) + (q*q)) - (f(r) + (r*r))) / (2 * q - 2 * r);
                // std::cout << "k: " << k << " s: " << s << " z[k]: " << z[k] << std::endl;
                std::cout << "s: " << s << std::endl;
                if (s > z[k]) {
                    done = true;
                }
                k--;
            }
        
            k++;
            v[k] = q;
            z[k] = s;
            z[k+1] = INT_MAX;
        }
    }


    k = 0;
    for (int i = 0; i < arr.rows; i++) {
        for (int j = 0; j < arr.cols; j++) {
            int q = arr.at<int>(i, j);
            while (z[k+1] < q) {
                k++;
            }
            int a = (q-v[k]);
            Df.at<int>(i, j) = int(a * a + f(v[k]));
        }
    }

    std::cout << "DF: " << Df << std::endl;
    return Df;
}

// Thus a two-dimensional distance transform can be computed by first computing
// one-dimensional distance transforms along each column of the grid, and then 
// computing one-dimensional distance transforms along each row of the result.
cv::Mat DTTwoDim(cv::Mat arr, std::function<int(int)> f) {
    try {
        if (arr.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Empty matrix in DTTwoDim." << std::endl;
        return arr;
    }

    for (int i = 0; i < arr.rows; i++) {
        for (int j = 0; j < arr.cols; j++) {
            // extract column and run one-dimensional distance transform 
            cv::Mat column = arr.col(j);
            cv::Mat transformed = DTOneDim(column, f);
            // replace column in original array
            column.col(0).copyTo(arr.col(j));
        }
        // extract row and run one-dimensional distance transform 
        cv::Mat row = arr.row(i);
        cv::Mat transformed = DTOneDim(row, f);
        // replace row in original array
        row.row(0).copyTo(arr.row(i));
    }

    return arr;
}

cv::Mat sample(cv::Mat img, std::string path, bool saving) {
    // read images and resize
    cv::Mat image;
    image = read(path, img);
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in sample." << std::endl;
        return image;
    }
    if (image.type() != 0) {
        if (image.type() == 16) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
            cv::threshold(image, image, 0, 255, cv::THRESH_BINARY);
        }
        // std::cout << "ERROR: Input image must be single chanel in offsetMap." << std::endl;
        // return dists;
    }

    cv::Mat sampled;
    // sampled =  DTTwoDim(image, f);
    sampled = DTOneDim(image, f);
    if (saving) {
        save(sampled, path, "-sampled");
    }

    return sampled;
}

// cv::Mat test(cv::Mat arr, std::string path) {
//     arr = read( path, arr);
//     cv::Mat column = arr.col(0);
//     for (int i = 0; i < column.rows; i++) {
//         for (int j = 0; j < column.cols; j++) {
//             column.at<uchar>(i, j) = uchar(10);
//         }
//     } 
//     column.col(0).copyTo(arr.col(0));
//     save(arr, path,  "-test");
//     return arr;
// }