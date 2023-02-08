#include "dots.hpp"

cv::Mat dots(std::string path, cv::Mat img, bool saving) {
    cv::Mat offsets = read(path, img);
    assert(!offsets.empty());

    if (offsets.type() != 4) {
        if (offsets.channels() != 1) {
            cv::cvtColor(offsets, offsets, cv::COLOR_RGB2GRAY);
        }
        offsets.convertTo(offsets, 4);
    }

    // step 0: generate seeds
    cv::Mat seeds = placeDots("", offsets, 6, true);

    int t1 = 1;
    int t2 = 0;
    while (t1 || t2) {
        // // step 1: get distance map
        cv::Mat3i dists = sample_seeds(seeds, "", false, true);
        // step 2: adjust seeds
        if (t1) {
            // no offset lane constraints
            seeds = adjust("", offsets, "", dists, false, false);
            t1--;
        } else {
            // alternate offset lane constraints
            seeds = adjust("", offsets, "", dists, false, t2 % 2);
            t2--;
        }
    }

    if (saving) {
        save(seeds, path, "-adjusted");
    }
    return seeds;
}