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
    
    cv::Mat edgedists = sample(edges, pathEdges, true);
    cv::Mat isosdists = sample(isos, pathIsos, true);

    cv::Mat distances = cv::Mat::zeros(edges.rows, edges.cols, CV_32SC1);
   
    int edge_weight = 1;
    int isos_weight = 2;

    for (int i = 0; i < distances.rows; i++) {
        for (int j = 0; j < distances.cols; j++) {
            float edge_dist = edgedists.at<int32_t>(i, j) / edge_weight;
            float isos_dist = isosdists.at<int32_t>(i, j) / isos_weight;
            if (isos_dist < edge_dist) {
                distances.at<int32_t>(i, j) = int32_t(isos_dist);
            } else {
                distances.at<int32_t>(i, j) = int32_t(edge_dist);
        }
    }
    // save image
    if (saving) {
        save(distances, pathEdges, "-dists-1");
    }

    distMap dists;
    dists.distances = distances;

    return dists;
}