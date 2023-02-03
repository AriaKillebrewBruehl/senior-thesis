#include "sampled-transform-2.hpp"

int32_t func(int32_t x) { return round(sqrt(x)); }
// indicator function for membership in a set of seed pixels
int32_t f(cv::Mat arr, int32_t p) {
    if (arr.type() != 4) {
        arr.convertTo(arr, 4);
    }

    try {
        if (arr.cols != 1) {
            throw arr.cols;
        }
    } catch (int i) {
        std::cout << "ERROR: Input matrix in f has " << i << " columns. "
                  << "Must be a single column matrix." << std::endl;
        return INT32_MAX;
    }

    int32_t value = int32_t(arr.at<int32_t>(p, 0)) == 255
                        ? INT_MAX
                        : int32_t(arr.at<int32_t>(p, 0));

    return value;
}

cv::Mat OneD(cv::Mat arr, std::function<int32_t(cv::Mat, int32_t)> f) {
    // type checking
    assert(!arr.empty());

    if (arr.type() != 4) {
        arr.convertTo(arr, 4);
    }

    try {
        if (arr.rows != 1 && arr.cols != 1) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Input matrix in OneD has dimensions " << arr.rows
                  << " x " << arr.cols
                  << ". Must be a single row or single column matrix."
                  << std::endl;
        return arr;
    }

    bool rotated = false;
    // rotate matrix so it is single column;
    if (arr.cols != 1) {
        cv::rotate(arr, arr, cv::ROTATE_90_CLOCKWISE);
        rotated = true;
    }
    assert(arr.cols == 1);

    cv::Mat final =
        cv::Mat::zeros(arr.rows, arr.cols, CV_32SC1);  // output matrix
    assert(final.type() == 4);
    int k = 0;  // index of right-most parabola in lower envelope
    std::vector<int> v{
        0};  // v[i] gives the horizontal position of the ith parabola aka our
             // position in our single column matrix
    std::vector<int> z{
        INT_MIN,
        INT_MAX};  // range in which the ith parabola of the lower envelope is
                   // below the others is given by z[i] and z[i+1]

    bool one_set = false;
    if (f(arr, 0) != INT32_MAX) {
        one_set = true;
    }

    for (int i = 1; i < arr.rows; i++) {
        int s;
        bool curr_inf = false;
        while (true) {
            int r = v[k];
            // current pixel is neither a seed pixel nor assigned a value
            if (f(arr, i) == INT32_MAX) {
                curr_inf = true;
                break;
            }
            // we know at least one pixel in the array has been set
            one_set = true;
            // intersection of parabola from i and r
            s = ((f(arr, i) + (i * i)) - (f(arr, r) + (r * r))) /
                (2 * i - 2 * r);

            if (s > z[k]) {
                break;
            }
            // if s <= z[k] then parabola from v[k] does not need to be part of
            // the lower envelope, so delete it by decreasing k
            v.erase(v.begin() + k);
            assert(!v.empty());
            z.erase(z.begin() + k);
            assert(!z.empty());
            k--;
        }
        // if the current parabola would be offset by infinity, don't add it and
        // don't change the existing lower envelope
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
        // while the range that the kth parabola covers is less than i increase
        // i
        while (z[k + 1] < i) {
            k++;
        }
        // if there were no seed pixels in the array make everything int_max
        if (v.size() == 1 && !one_set) {
            final.at<int32_t>(i, 0) = 255 * 255;
            continue;
        }
        if (f(arr, i) == 0) {
            continue;
        }
        // distance between i and the horizontal position of the kth parabola
        int32_t a = abs(i - v[k]);
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
    assert(!arr.empty());

    if (arr.type() != 4) {
        arr.convertTo(arr, 4);
    }

    for (int j = 0; j < arr.cols; j++) {
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
    save(arr, "", "-w-rows");

    return arr;
}

cv::Mat sample(cv::Mat img, std::string path, bool saving) {
    // read images and resize
    cv::Mat image;
    image = read(path, img);

    assert(!image.empty());

    if (image.type() != 4) {
        image.convertTo(image, 4);
    }

    cv::Mat sampled;
    sampled = TwoD(image, f);

    std::transform(sampled.begin<int32_t>(), sampled.end<int32_t>(),
                   sampled.begin<int32_t>(), func);

    if (saving) {
        save(sampled, path, "-sampled");
    }

    return sampled;
}