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