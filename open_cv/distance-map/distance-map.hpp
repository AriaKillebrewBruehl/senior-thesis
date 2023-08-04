#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>

#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "sampled-transform-seeds.hpp"

// parameters:
//
// pathEdges - the file path to the input edges
// (can be "" if imgEdges is not empty)
//
// imgEdges - the edges as a cv::Mat
// (can be empty if pathEdges is not "")
//
// pathIsos - the file path to the input isophotes
// (can be "" if imgIsos is not empty)
//
// imgIsos - the isohpotes as a cv::Mat
// (can be empty if pathIsos is not "")
//
// saving - saves the generated distance map to the file system
cv::Mat distanceMap(std::string pathEdges, cv::Mat imgEdges,
                    std::string pathIsos, cv::Mat imgIsos, bool saving);