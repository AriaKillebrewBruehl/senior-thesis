#include "fullMap.hpp"

cv::Mat fullMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos,
                cv::Mat imgIsos, bool saving) {
    // read images and resize
    cv::Mat edges;
    edges = read(pathEdges, imgEdges);
    assert(!edges.empty());
    // assert(edges.depth() == 0);

    cv::Mat inverted_edges = invert(edges);

    if (inverted_edges.type() != 4) {
        if (inverted_edges.channels() != 1) {
            cv::cvtColor(inverted_edges, inverted_edges, cv::COLOR_RGB2GRAY);
        }
        inverted_edges.convertTo(inverted_edges, 4);
    }

    cv::Mat isos;
    isos = read(pathIsos, imgIsos);
    assert(!isos.empty());

    // assert(isos.depth() == 0);

    cv::Mat inverted_isos = invert(isos);

    if (inverted_isos.type() != 4) {
        if (inverted_isos.channels() != 1) {
            cv::cvtColor(inverted_isos, inverted_isos, cv::COLOR_RGB2GRAY);
        }
        inverted_isos.convertTo(inverted_isos, 4);
    }

    cv::Mat distances =
        distanceMap("", inverted_edges, "", inverted_isos, false);
    cv::Mat oMap = offsetMap(pathEdges, distances, saving, true);
    return oMap;
}