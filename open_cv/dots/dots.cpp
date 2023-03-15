#include "dots.hpp"

cv::Mat dots(std::string pathOffset, cv::Mat imgOffset, std::string pathSeeds,
             cv::Mat imgSeeds, int d, bool saving) {
    cv::Mat offsets = read(pathOffset, imgOffset);
    assert(!offsets.empty());

    if (offsets.type() != 4) {
        if (offsets.channels() != 1) {
            cv::cvtColor(offsets, offsets, cv::COLOR_RGB2GRAY);
        }
        offsets.convertTo(offsets, 4);
    }

    cv::Mat seeds = read(pathSeeds, imgSeeds);
    assert(!seeds.empty());

    if (seeds.type() != 4) {
        if (seeds.channels() != 1) {
            cv::cvtColor(seeds, seeds, cv::COLOR_RGB2GRAY);
        }
        seeds.convertTo(seeds, 4);
    }

    int t1 = 10;
    int t2 = 30;
    while (t1 || t2) {
        // step 1: generate distance map
        cv::Mat3i dists = sample_seeds(seeds, "", false, true);

        // step 2: adjust seed placement
        if (t1) {
            // no offset lane constraints
            seeds = adjust("", offsets, "", dists, false, false);
            t1--;
        }
        if (!t1 && t2) {
            // alternate offset lane constraints
            seeds = adjust("", offsets, "", dists, false, t2 % 2);
            t2--;
        }
    }

    if (saving) {
        save(seeds, pathSeeds, "-adjusted");
    }
    return seeds;
}

// cv::Mat dots(std::string path, cv::Mat img, int d, bool saving) {
//     cv::Mat offsets = read(path, img);
//     assert(!offsets.empty());

//     if (offsets.type() != 4) {
//         if (offsets.channels() != 1) {
//             cv::cvtColor(offsets, offsets, cv::COLOR_RGB2GRAY);
//         }
//         offsets.convertTo(offsets, 4);
//     }

//     // step 0: generate seeds
//     cv::Mat seeds = placeSeeds(path, offsets, 6, true);

//     int t1 = 10;
//     int t2 = 30;
//     while (t1 || t2) {
//         // step 1: generate distance map
//         cv::Mat3i dists = sample_seeds(seeds, "", false, true);

//         // step 2: adjust seed placement
//         if (t1) {
//             // no offset lane constraints
//             seeds = adjust("", offsets, "", dists, false, false);
//             t1--;
//         }
//         if (t1 == 0 && t2 == 30) {
//             save(seeds, path, "-after-t1");
//         }
//         if (!t1 && t2) {
//             // alternate offset lane constraints
//             seeds = adjust("", offsets, "", dists, false, t2 % 2);
//             t2--;
//         }
//     }

//     if (saving) {
//         save(seeds, path, "-adjusted");
//     }
//     return seeds;
// }