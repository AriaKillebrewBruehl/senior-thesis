#include "dots.hpp"

cv::Mat dots(std::string pathOffset, cv::Mat imgOffset, std::string pathSeeds,
             cv::Mat imgSeeds, int d, bool saving) {
    cv::Mat offsets = read(pathOffset, imgOffset);
    assert(!offsets.empty());

    if (offsets.type() != 4) {
        if (offsets.channels() == 3) {
            cv::cvtColor(offsets, offsets, cv::COLOR_RGB2GRAY);
        } else if (offsets.channels() == 4) {
            cv::cvtColor(offsets, offsets, cv::COLOR_RGBA2GRAY);
        }
        offsets.convertTo(offsets, 4);
    }

    cv::Mat seeds = read(pathSeeds, imgSeeds);
    assert(!seeds.empty());

    if (seeds.type() != 4) {
        if (seeds.channels() == 3) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGB2GRAY);
        } else if (seeds.channels() == 4) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGBA2GRAY);
        }
        seeds.convertTo(seeds, 4);
    }

    int t1 = 10;
    int t2 = 30;
    while (t1 || t2) {
        // step 1: generate voronoi  diagram of seed pixels
        cv::Mat3i voronoi = sample_seeds(seeds, "", false, true);

        // step 2: adjust seed placement
        if (t1) {
            // no offset lane constraints
            seeds = adjust("", offsets, "", voronoi, false, false);
            t1--;
        }
        if (!t1 && t2) {
            // alternate offset lane constraints
            // seeds = adjust("", offsets, "", voronoi, false, t2 % 2);
            seeds = adjust("", offsets, "", voronoi, false, true);
            t2--;
        }
    }

    if (saving) {
        save(seeds, pathSeeds, "-adjusted");
    }
    return seeds;
}