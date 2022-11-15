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
    if (edges.type() == 0) {
        cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
    }
    resz(edges);
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
    if (isos.type() == 0) {
        cv::cvtColor(isos, isos, cv::COLOR_GRAY2BGR);
    }
    resz(isos);

    distMap dMap = distanceMap("", edges, "", isos, false);
    cv::Mat distances = dMap.distances;
    cv::Mat priorities = dMap.priorityBuffer;

    cv::Mat oMap = offsetMap("", distances, "", priorities, false);

    cv::Mat edgesIsos = combine("", edges, "", isos, false);
    cv::bitwise_not(edgesIsos, edgesIsos);

    cv::Mat final = combine("", edgesIsos, "", oMap, false);

    if (saving) {
        save(final, pathEdges, "-fullMap");
    }
    return final;
}