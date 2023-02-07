#include "dots.hpp"

cv::Mat dots(std::string path, cv::Mat img, bool saving) {
    cv::Mat image = read(path, img);
    assert(!image.empty());

    // TODO type checking

    // step 0: generate seeds
    cv::Mat seeds = placeDots("", image, 6, true);

    int t1 = 10;
    int t2 = 30;
    cv::Mat adjusted;
    while (t1 || t2) {
        // step 1: get distance map
        cv::Mat3i dist = sample_seeds(seeds, "", false, true);
        // step 2: adjust seeds
        if (t1) {
            // no offset lane constraints
            adjusted = adjust("", image, "", dist, false, false);
            t1--;
        } else {
            // alternate offset lane constraints
            adjusted = adjust("", image, "", dist, false, t2 % 2);
            t2--;
        }
    }

    if (saving) {
        save(adjusted, path, "-adjusted");
    }
    return adjusted;
}