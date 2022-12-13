#include "sampled-transform.hpp"

// indicator function for membership in a set of seed pixels
int32_t f(cv::Mat arr, int32_t p) {
    try {
        if (arr.type() != 4) {
            throw arr.type();
        }
    } catch (int t) {
        std::cout << "ERROR: matrix passed to f was of type " << t << ".\n";
        std::cout << "Function f only accepts matrices of type 4." << std::endl;
        return INT32_MAX;
    }
    try {
        if (arr.cols != 1) {
            throw arr.cols;
        }
    } catch (int i) {
        std::cout << "ERROR: Input matrix in DTOneDim has " << i << " columns. " << "Must be a single column matrix." << std::endl;
        return INT32_MAX;
    } 

    int32_t value = int32_t(arr.at<int32_t>(p, 0)) == 255 ? INT_MAX: int32_t(arr.at<int32_t>(p, 0));
    // int32_t value = int32_t(arr.at<int32_t>(p, 0));
    // if (value == 255) {
    //     return INT32_MAX;
    // }
    return value;
}

cv::Mat OneD(cv::Mat arr, std::function<int32_t(cv::Mat, int32_t)> f) {
    // type checking
    {
        try {
            if (arr.empty()) {
                throw 0;
            }
        } 
        catch (int i) {
            std::cout << "ERROR: Empty matrix in OneD." << std::endl;
            return arr;
        }
        
        try {
            if (arr.type() != 4) {
                throw arr.type();
            }
        } 
        catch (int i) {
            std::cout << "ERROR: Input matrix in OneD must be of type 4 (32S_C1)." << std::endl; 
            std::cout << "Input matrix was of type: " << i << std::endl;
            return arr;
        }
        
        try {
            if (arr.rows != 1 && arr.cols != 1) {
                throw 0;
            }
        } 
        catch (int i) {
            std::cout << "ERROR: Input matrix in OneD has dimensions " << arr.rows << " x " << arr.cols << ". Must be a single row or single column matrix." << std::endl;
            return arr;
        }
    }

    bool rotated = false;
    // rotate matrix so it is single column;
    if (arr.cols != 1) {
        cv::rotate(arr, arr, cv::ROTATE_90_CLOCKWISE);
        rotated = true;
    }
    assert(arr.cols == 1);

    cv::Mat final = cv::Mat::zeros(arr.rows, arr.cols, CV_32SC1); // output matrix 
    assert(final.type() == 4);
    int k = 0; // index of right-most parabola in lower envelope 
    std::vector<int> v{0}; // v[i] gives the horizontal position of the ith parabola aka our position in our single column matrix 
    std::vector<int> z{INT_MIN, INT_MAX}; // range in which the ith parabola of the lower envelope is below the others is given by z[i] and z[i+1]

    bool one_set = false;
    if (f(arr, 0) != INT32_MAX) {
        one_set = true;
    }

    for (int i = 1; i < arr.rows; i++) {
        int s;
        bool curr_inf = false;
        while (true) {
            int  r = v[k];
            // current pixel is neither a seed pixel nor assigned a value
            if (f(arr, i) == INT32_MAX) {
                curr_inf = true;
                break;
            }
            // we know at least one pixel in the array has been set
            one_set = true;
            // intersection of parabola from i and r
            s = ((f(arr, i) + (i*i)) - (f(arr, r) + (r*r))) / (2 * i - 2 * r);
           
            if (s > z[k]) {
                break;
            }
            // if s >= z[k] then parabola from v[k] does not need to be part of the lower envelope, so delete it by decreasing k
            v.erase(v.begin() + k);
            assert(!v.empty());
            k--;
        }
        // if the current parabola would be offset by infinity, don't add it and don't change the existing lower envelope
        if (curr_inf) {
            continue;
        }
        // otherwise modify lower envelope 
        // increase k
        k++;
        // add i as the kth parabola
        v.push_back(i);
        // make i is below the others starting at s
        z[k] = s;
        // and ending at infinity
        z.push_back(INT_MAX);
    }
    

    k = 0;
    for (int i = 0; i < arr.rows; i++) {
        // while the range that the kth parabola covers is less than i increase i 
        while (z[k+1] < i) {
            k++;
        }
        // if there were no seed pixels in the array make everything int_max
        if (v.size() == 1 && !one_set) {
            final.at<int32_t>(i, 0) = INT32_MAX;
            continue;
        }
        // distance between i and the horizontal position of the kth parabola 
        int32_t a = abs(i-v[k]);
        int32_t b = f(arr, v[k]);
        int32_t value = a * a + b;
        
        final.at<int32_t>(i, 0) = (value);
    }

    if (rotated) {
        cv::rotate(final, final, cv::ROTATE_90_COUNTERCLOCKWISE);
    }
    assert(final.type() == 4);
    return final;
}

cv::Mat TwoD(cv::Mat arr, std::function<int32_t(cv::Mat, int32_t)> f) {
    // type check
    {
        try {
            if (arr.empty()) {
                throw 0;
            }
        } catch (int i) {
            std::cout << "ERROR: Empty matrix in DTTwoDim." << std::endl;
            return arr;
        }
        try {
            if (arr.type() != 4) {
                throw arr.type();
            }
        } catch (int i) {
            std::cout << "ERROR: Input matrix in OneD must be of type 4 (32S_C1)." << std::endl; 
            std::cout << "Input matrix was of type: " << i << std::endl;
            return arr;
        }
    }
    for (int j = 0; j < arr.cols; j++) {
        std::cout << std::endl << "row " << j << std::endl;
        // extract column and run one-dimensional distance transform 
        cv::Mat column = arr.col(j);
        assert(column.type() == 4);
        cv::Mat transformed = OneD(column, f);
        assert(column.type() == 4);
        // replace column in original array
        transformed.col(0).copyTo(arr.col(j));
    }

    for (int i = 0; i < arr.rows; i++) {
        // extract row and run one-dimensional distance transform 
        cv::Mat row = arr.row(i);
        assert(row.type() == 4);
        cv::Mat transformed = OneD(row, f);
        assert(row.type() == 4);
        // replace row in original array
        transformed.row(0).copyTo(arr.row(i));
    }

    return arr;
}

cv::Mat sample(cv::Mat img, std::string path, bool saving) {
    // read images and resize
    cv::Mat image;
    image = read(path, img);
    cv::Mat correct;
    
    try {
        if (image.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in sample." << std::endl;
        return image;
    }
    
    if (image.type() != 4) {
        // convert color image to binary
        if (image.type() == 16) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
            cv::threshold(image, image, 0, 255, cv::THRESH_BINARY);
        } 
    
        try{
            if (image.channels() != 1) {
                throw image.channels();
            }
        } catch (int j) {
            std::cout << "ERROR: Input image in sample must be single chanel" << std::endl;
            std::cout << "Input image has " << j << " chanels" << std::endl;
            return image;
        }

        try {
            image.convertTo(correct, CV_32SC1);
            if (correct.type()!= 4) {
                throw image.type();
            }
        } catch (int i) {
            std::cout << "ERROR: Input image in sample could not convert to type CV_32SC1." << std::endl;
            std::cout << "Input image has type " << i << "." << std::endl;
            return image;
        } 
    } else {
        correct = image;
    }
    
    assert(correct.type() == 4);
    cv::Mat sampled;
    sampled =  TwoD(correct, f);
    std::cout << std::endl << std::endl;
    for (int i = 0; i < sampled.rows; i++) {
        for (int j = 0; j < sampled.cols ; j++) {
            sampled.at<int32_t>(i, j) = round(sqrt(sampled.at<int32_t>(i, j)));
            std::cout << sampled.at<int32_t>(i, j) << " ";
        }
        std::cout << std::endl;
    }

    if (saving) {
        save(sampled, path, "-sampled-2");
    }

    return sampled;
}