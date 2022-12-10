#include "distance-map2.hpp"

distMap distanceMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos, cv::Mat imgIsos, bool saving) {
    // read images and resize
    cv::Mat edges;
    edges = read(pathEdges, imgEdges);
    try {
        if (edges.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in distanceMap." << std::endl;
        distMap dMap;
        return dMap;
    }
    if (edges.type() == 0) {
        cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
    }
    cv::Mat isos;
    isos = read(pathIsos, imgIsos);
     try {
        if (isos.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in distanceMap." << std::endl;
        distMap dMap;
        return dMap;
    }
    if (isos.type() == 0) {
        cv::cvtColor(isos, isos, cv::COLOR_GRAY2BGR);
    }
    
    cv::Mat edgedists = sample(edges, "", false);
    cv::Mat isosdists = sample(isos, "", false);

    cv::Mat distances = cv::Mat::zeros(edges.rows, edges.cols, CV_8UC1);
    cv::Mat priorityBuffer = cv::Mat::zeros(edges.rows, edges.cols, CV_8UC1);

    int edge_weight = 1;
    int isos_weight = 2;
    for (int i = 0; i < distances.rows; i++) {
        for (int j = 0; j < distances.cols; j++) {
    
            float edge_dist = edgedists.at<float32_t>(i, j) / edge_weight;
            float isos_dist = isosdists.at<float32_t>(i, j) / isos_weight;
            if (isos_dist < edge_dist) {
                isos_dist = int(isos_dist) % 255;
                distances.at<uchar>(i, j) = uchar(isos_dist);
                priorityBuffer.at<uchar>(i, j) = uchar(isos_weight);
            } else {
                edge_dist = int(edge_dist) % 255;
                distances.at<uchar>(i, j) = uchar(edge_dist);
                priorityBuffer.at<uchar>(i, j) = uchar(edge_weight);
            }
        }
    }
    // save image
    if (saving) {
        save(distances, pathEdges, "-dists-0");
        save(priorityBuffer, pathEdges, "-priorities-0");
    }

    distMap dists;
    dists.distances = distances;
    dists.priorityBuffer = priorityBuffer;

    return dists;
}