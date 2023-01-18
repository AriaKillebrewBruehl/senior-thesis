#include "fullMap.hpp"

cv::Mat fullMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos, cv::Mat imgIsos, bool saving) {
    // read images and resize
    cv::Mat edges;
    edges = read(pathEdges, imgEdges);
    try {
        if (edges.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in distanceMap." << std::endl;
        return edges;
    }

    assert(edges.depth() == 0);

    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =  cv::connectedComponentsWithStats(edges, labels, stats, centroids);

    int i;
    int j;
    for (i = 0; i < labels.rows; i++) {
        for (j = 0; j < labels.cols; j++) {
            if (labels.at<int>(i, j) == 0) {
                break;
            }
        }
    }

    // background is black so invert
    if (edges.at<int>(i,j) == 0) {
        cv::bitwise_not(edges, edges);
    }

    if (edges.type() == 16) {
        cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
    } 
    if (edges.type() != 4) {
        edges.convertTo(edges, CV_32SC1);
    }
    
    cv::Mat isos;
    isos = read(pathIsos, imgIsos);
     try {
        if (isos.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in distanceMap." << std::endl;
        return edges;
    }

    assert(isos.depth() == 0);

    // get components
    numComps =  cv::connectedComponentsWithStats(isos, labels, stats, centroids);

    for (i = 0; i < labels.rows; i++) {
        for (j = 0; j < labels.cols; j++) {
            if (labels.at<int>(i, j) == 0) {
                break;
            }
        }
    }

    // background is black so invert
    if (isos.at<int>(i,j) == 0) {
        cv::bitwise_not(isos, isos);
    }

    if (isos.type() == 16) {
        cv::cvtColor(isos, isos, cv::COLOR_GRAY2BGR);
    } 
    if (isos.type() != 4) {
        isos.convertTo(isos, CV_32SC1);
    }

    distMap dMap = distanceMap("", edges, "", isos, false);
    cv::Mat distances = dMap.distances;
    cv::Mat priorities = dMap.priorityBuffer;

    distances.convertTo(distances, CV_8UC1);
    cv::Mat oMap = offsetMap(pathEdges, distances, saving);
    return oMap;
}