#include "sampled-transform.hpp"

// indicator function for membership in a set of seed pixels
int f(cv::Mat arr, int p) {
    try {
        if (arr.type() != 0) {
            throw arr.type();
        }
    } catch (int t) {
        std::cout << "ERROR: matrix passed to f was of type " << t << ".\n";
        std::cout << "Function f only accepts matrices of type 0." << std::endl;
        return INT_MAX;
    }
    try {
        if (arr.cols != 1) {
            throw arr.cols;
        }
    } catch (int i) {
        std::cout << "ERROR: Input matrix in DTOneDim has " << i << " columns. " << "Must be a single column matrix." << std::endl;
        return INT_MAX;
    } 

    int value = int(arr.at<uchar>(p, 0));
    // std::cout << "f(" << p << ") = ";
    if (value == 255) {
        // std::cout << "255" << std::endl;
        return 0;
    }
    // std::cout << "INT_MAX" << std::endl;

    return INT_MAX;
}

cv::Mat DTOneDim(cv::Mat arr, std::function<int(cv::Mat, int)> f) {
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

    bool rotated = false;
    // multiple columns in one row 
    if (arr.cols != 1) {
        // rotate matrix 90 degrees to create a single column matrix 
        cv::rotate(arr, arr, cv::ROTATE_90_CLOCKWISE);
    }
    assert(arr.cols == 1);

    std::cout << arr << std::endl;


    cv::Mat Df = cv::Mat::zeros(arr.rows, arr.cols, CV_8UC1); // output matrix 
    int k = 0; // index of right-most parabola in lower envelope 
    std::vector<int> v{0}; // v[i] gives the horizontal position of the ith parabola aka our position in our single column matrix 
    std::vector<int> z{INT_MIN, INT_MAX}; // range in which the ith parabola of the lower envelope is below the others is given by z[i] and z[i+1]

    // single column matrix so just loop over the rows
    for (int i = 1; i < arr.rows; i++) {
        std::cout << std::endl;
        std::cout << "analyzing pixel " << i << std::endl;
        std::cout << "arr(i) has value: " << int(arr.at<uchar>(i, 0)) << std::endl;
       
        // std::cout << "(" << i << ", " << j << ") = " << q << ", f(q) = " << f(q) << std::endl;
        
        int s;
        bool curr_inf = false;
        while (true) {
            int  r = v[k];
            std::cout << "comparing to " << k << "th parabola with horizontal position " << r << std::endl;
            std::cout << "f(i) = " << f(arr, i) << " f(v[k]) = " << f(arr, r) << std::endl;
            if (f(arr, i) == INT_MAX) {
                curr_inf = true;
                break;
            }
            // intersection of parabola from i and r
            s = ((f(arr, i) + (i*i)) - (f(arr, r) + (r*r))) / (2 * i - 2 * r);
            std::cout << "s: " << s << std::endl;
            std::cout << "z[k]: " << z[k] << " z[k+1]: " << z[k+1] << std::endl;
            if (s > z[k]) {
                break;
            }
            // if s >= z[k] then parabola from v[k] does not need to be part of the lower envelope, so delete it by decreasing k
            std::cout << "removing parabola k" << std::endl;
            k--;
        }
        if (curr_inf) {
            std::cout << "current parabola has inifinte vertical" << std::endl;
            continue;
        }
        std::cout << "adding new parabola" << std::endl;
        // otherwise modify lower envelope 
        // increase k
        k++;
        std::cout << "increased k to: " << k << std::endl;
        // add i as the kth parabola
        v.push_back(i);
        std::cout << "updated v, v[k] = " << v[k] << std::endl;
        // make i is below the others starting at s
        z[k] = s;
        // and ending at infinity
        z.push_back(INT_MAX);
         std::cout << "updated z, z[k] = " << z[k] << " z[k + 1] = " << z[k+1] << std::endl;
        std::cout << std::endl;
    }

    std::cout << "final v has size " << v.size() << " v: " ;
    for (int i: v) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "final z: ";
    for (int i: z) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;


    k = 0;
    for (int i = 0; i < arr.rows; i++) {
        // while the range that the kth parabola covers is less than i increase i 
        while (z[k+1] < i) {
            k++;
        }
        // distance between i and the horizontal position of the kth parabola 
        int a = abs(i-v[k]);
        Df.at<uchar>(i, 0) = uchar(a + f(arr, v[k]));
    }

    if (rotated) {
        cv::rotate(Df, Df, cv::ROTATE_90_COUNTERCLOCKWISE);
    }

    std::cout << "DF: " << Df << std::endl;
    return Df;
}

// Thus a two-dimensional distance transform can be computed by first computing
// one-dimensional distance transforms along each column of the grid, and then 
// computing one-dimensional distance transforms along each row of the result.
cv::Mat DTTwoDim(cv::Mat arr, std::function<int(cv::Mat, int)> f) {
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
    sampled =  DTOneDim(image, f);

    /*for (int i = 0; i < sampled.rows; i++) {
        for (int j = 0; j < sampled.cols ; j++) {
            std::cout << sampled.at<int>(0, j);
        }
        std::cout << std::endl;
     }*/
    // sampled = DTOneDim(image, f);
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