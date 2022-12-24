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
    if (value == 255) {
        return INT_MAX;
    }

    return value;
}

cv::Mat DTOneDim(cv::Mat arr, std::function<int(cv::Mat, int)> f, int pass) {
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
        rotated = true;
    }
    assert(arr.cols == 1);

    // std::cout << "input array: " << arr << std::endl;

    cv::Mat Df = cv::Mat::zeros(arr.rows, arr.cols, CV_8UC1); // output matrix 
    int k = 0; // index of right-most parabola in lower envelope 
    std::vector<int> v{0}; // v[i] gives the horizontal position of the ith parabola aka our position in our single column matrix 
    std::vector<int> z{INT_MIN, INT_MAX}; // range in which the ith parabola of the lower envelope is below the others is given by z[i] and z[i+1]

    bool one_set = false;
    // std::cout << "f(arr, 0) = " << f(arr, 0) << std::endl;
    if (f(arr, 0) != INT_MAX) {
        one_set = true;
    }
    // single column matrix so just loop over the rows
    for (int i = 1; i < arr.rows; i++) {
        // std::cout << std::endl;
        // std::cout << "analyzing pixel " << i << std::endl;
        // std::cout << "arr(i) has value: " << int(arr.at<uchar>(i, 0)) << std::endl;
        
        int s;
        bool curr_inf = false;
        while (true) {
            int  r = v[k];
            // std::cout << "comparing to " << k << "th parabola with horizontal position " << r << std::endl;
            // std::cout << "f(i) = " << f(arr, i) << " f(v[k]) = " << f(arr, r) << std::endl;
            // current pixel is neither a seed pixel nor assigned a value
            if (f(arr, i) == INT_MAX) {
                curr_inf = true;
                break;
            }
            // we know at least one pixel in the array has been set
            one_set = true;
            // intersection of parabola from i and r
            s = ((f(arr, i) + (i*i)) - (f(arr, r) + (r*r))) / (2 * i - 2 * r);
            // std::cout << "s: " << s << std::endl;
            // std::cout << "z[k]: " << z[k] << " z[k+1]: " << z[k+1] << std::endl;
            if (s > z[k]) {
                break;
            }
            // if s >= z[k] then parabola from v[k] does not need to be part of the lower envelope, so delete it by decreasing k
            // std::cout << "removing parabola k" << std::endl;
            k--;
        }
        // if the current parabola would be offset by infinity, don't add it and don't change the existing lower envelope
        if (curr_inf) {
            continue;
        }
        // std::cout << "adding new parabola" << std::endl;
        // otherwise modify lower envelope 
        // increase k
        k++;
        // std::cout << "increased k to: " << k << std::endl;
        // add i as the kth parabola
        v.push_back(i);
        // std::cout << "updated v, v[k] = " << v[k] << std::endl;
        // make i is below the others starting at s
        z[k] = s;
        // and ending at infinity
        z.push_back(INT_MAX);
        // std::cout << "updated z, z[k] = " << z[k] << " z[k + 1] = " << z[k+1] << std::endl;
        // std::cout << std::endl;
    }

    std::cout << " v: [";
    for (int i : v) {
        std::cout << i << ", ";
    }
    std::cout << "]" << std::endl;
     std::cout << " z: [";
    for (int i : z) {
        std::cout << i << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << std::endl;

    k = 0;
    for (int i = 0; i < arr.rows; i++) {
        // while the range that the kth parabola covers is less than i increase i 
        while (z[k+1] < i) {
            std::cout << "i: " << i << " k: " << k << " z[k+1]: " << z[k+1] << std::endl;
            k++;
        }
        // if there were no seed pixels in the array make everything 255
        if (v.size() == 1 && !one_set) {
            Df.at<uchar>(i, 0) = 255;
            continue;
        }
        // distance between i and the horizontal position of the kth parabola 
        int a = abs(i-v[k]);
        int b = f(arr, v[k]);
        int value;
        if (b != 0) {
            // int value1;
            // if (k != 0) {
            //     int c = abs(i-v[k-1]);
            //     int d = f(arr, v[k-1]);
            //     if (d != 0) {
            //         value1 = int(sqrt(c*c) + (d*d));
            //     } else {
            //         value1 = c + d;
            //     }
            // } else {
                value = int(sqrt((a*a) + (b*b)));
            // }
            // (value0 < value1) ? value0 = value0 : value0 = value1;    
                
        } else {
            value = a + b;
        }

        

        // std::cout << "i: " << i << " value0: " << value0 << " value1: " << value1 << std::endl;
        // int value;
        

        // if (i == z[k+1]) {
        //     int r = v[k + 2];
        //     if (r != INT_MAX) {
        //         int  s = ((f(arr, i) + (i*i)) - (f(arr, r) + (r*r))) / (2 * i - 2 * r);
        //         z[k + 2] = s;
        //     }
        //     z.erase(z.begin()+k+1);   
        // }
        // if (i == 0) {
            // std::cout << "a: " << a << " k: " << k << " v[k]: " << v[k] << " f(v[k]): " << f(arr, v[k]) << " value: " << value << std::endl;
            // std::cout << std::endl;
        //}
        Df.at<uchar>(i, 0) = uchar(value);
    }

    if (rotated) {
        cv::rotate(Df, Df, cv::ROTATE_90_COUNTERCLOCKWISE);
    }

    // std::cout << "DF: " << Df << std::endl;
   
    return Df;
}

cv::Mat DTTwoDim(cv::Mat arr, std::function<int(cv::Mat, int)> f) {
    try {
        if (arr.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Empty matrix in DTTwoDim." << std::endl;
        return arr;
    }

    for (int j = 0; j < arr.cols; j++) {
            // extract column and run one-dimensional distance transform 
            // if (j != 2) {
            //     continue;
            // }
            cv::Mat column = arr.col(j);
            cv::Mat transformed = DTOneDim(column, f, 0);
            // replace column in original array
            transformed.col(0).copyTo(arr.col(j));
        }


    for (int i = 0; i < arr.rows; i++) {
        for (int j = 0; j < arr.cols ; j++) {
            std::cout << int(arr.at<uchar>(i, j)) << " ";
        }
        std::cout << std::endl;
     }

     std::cout << std::endl;

    //  save(arr, "", "sampled-cols");

    for (int i = 0; i < arr.rows; i++) {
        
        // for (int i = 0; i < arr.rows; i++) {
        // for (int j = 0; j < arr.cols ; j++) {
        //     std::cout << int(arr.at<uchar>(i, j)) << " ";
        // }
        // std::cout << std::endl;
     // }
        // extract row and run one-dimensional distance transform 
        cv::Mat row = arr.row(i);
        cv::Mat transformed = DTOneDim(row, f, 1);
        // replace row in original array
        transformed.row(0).copyTo(arr.row(i));
        // save(arr, "", "sampled-rows");
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
    try {
        if (image.type() != 0) {
           throw image.type();
        } 
    } catch (int i) {
         if (i == 16) {
                cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
                cv::threshold(image, image, 0, 255, cv::THRESH_BINARY);
        } else {
            std::cout << "ERROR: Input image must be single chanel in sample or 8bit 3 channel." << std::endl;
            return image;
        }
    }
    if (image.type() != 0) {
        if (image.type() == 16) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
            cv::threshold(image, image, 0, 255, cv::THRESH_BINARY);
        }
        
        // std::cout << "ERROR: Input image must be single chanel in offsetMap." << std::endl;
        return image;
    }
    cv::Mat sampled;
    sampled =  DTTwoDim(image, f);
    for (int i = 0; i < sampled.rows; i++) {
        for (int j = 0; j < sampled.cols ; j++) {
            std::cout << int(sampled.at<uchar>(i, j)) << " ";
        }
        std::cout << std::endl;
     }
  
    if (saving) {
        save(sampled, path, "-sampled-both");
    }

    return sampled;
}