#include "offset-map.hpp"

cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, std::string pathPriorities, cv::Mat imgPriorities, bool saving) {
    // read images and resize
    cv::Mat dists;
    dists = read(pathDists, imgDists);
    cv::Mat priorities;
    priorities = read(pathPriorities, imgPriorities);

    cv::Mat offsetMap(dists.rows, dists.cols, CV_8UC3, cv::Scalar(255));

    int l = 6;
    int w0 = 1;
    int id0 = 0;
    int Rmin = ((l - w0) * id0) - int(w0/2);
    int Rmax = ((l - w0) * id0) + int(w0/2);
    for (int i = 0; i < offsetMap.rows; i++) {
        for (int j = 0; j < offsetMap.cols; j++) {
            int df = int(dists.at<cv::Vec3b>(i, j)[0]);
            int pf = int(priorities.at<cv::Vec3b>(i, j)[0]);

            if (df * pf >= Rmin && df * pf <= Rmax) {
                offsetMap.at<cv::Vec3b>(i, j)[0] = 0;
                offsetMap.at<cv::Vec3b>(i, j)[1] = 0;
                offsetMap.at<cv::Vec3b>(i, j)[2] = 0;
            }
        }
    }

    if (saving) {
        save(offsetMap, pathDists, "-o-map");
    }
    return offsetMap;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run jfa on." << std::endl;
    } else {
       for (int i = 1; i < argc; i += 2) {
            cv::Mat image;
            offsetMap(argv[i], image, argv[i+1], image, true);
       }
    }
}