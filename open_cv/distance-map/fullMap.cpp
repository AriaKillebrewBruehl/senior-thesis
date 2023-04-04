#include "fullMap.hpp"

cv::Mat fullMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos,
                cv::Mat imgIsos, float l, bool offset_sections, bool saving) {
    // read images and resize
    cv::Mat edges;
    edges = read(pathEdges, imgEdges);
    assert(!edges.empty());

    cv::Mat inverted_edges = invert(edges);

    if (inverted_edges.type() != 4) {
        if (inverted_edges.channels() == 4) {
            cv::cvtColor(inverted_edges, inverted_edges, cv::COLOR_RGBA2GRAY);
        } else if (inverted_edges.channels() == 3) {
            cv::cvtColor(inverted_edges, inverted_edges, cv::COLOR_RGB2GRAY);
        }
        inverted_edges.convertTo(inverted_edges, 4);
    }

    cv::Mat isos;
    isos = read(pathIsos, imgIsos);
    assert(!isos.empty());

    cv::Mat inverted_isos = invert(isos);

    if (inverted_isos.type() != 4) {
        if (inverted_isos.channels() == 4) {
            cv::cvtColor(inverted_isos, inverted_isos, cv::COLOR_RGBA2GRAY);
        } else if (inverted_isos.channels() == 3) {
            cv::cvtColor(inverted_isos, inverted_isos, cv::COLOR_RGB2GRAY);
        }
        inverted_isos.convertTo(inverted_isos, 4);
    }

    cv::Mat distances =
        distanceMap(pathEdges, inverted_edges, pathIsos, inverted_isos, false);
    cv::Mat details = cv::Mat::zeros(distances.size(), CV_8UC1);
    cv::Mat oMap = offsetMap(pathEdges, distances, "", details, l, saving,
                             offset_sections);
    return oMap;
}