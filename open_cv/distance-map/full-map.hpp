#pragma once
#include "../boiler-plate/invert.hpp"
#include "distance-map.hpp"
#include "offset-map.hpp"

// Returns a distance map from the input edges and isophotes
// parameters:
// pathEdges - the path to the input edges
// (can be "" is imgEdges is not empty)
//
// imgEdges - the input edges as a cv::Mat
// (can be empty if pathEdges is not "")
//
// pathIsos - the path to the input isophotes
// (can be "" is imgIsos is not empty)
//
// imgIsos - the input isophotes as a cv::Mat
// (can be empty if pathIsos is not "")
//
// l - the integer distance between offset lines
//
// offset_sections - if true returns an offset map with
// black offset lines and sections of color corresponding to the section id,
// if false, returns an offset map with a white background and offset
// lines of color corrsponding to the line's id
//
// saving - if true saves the generated offset map to the file system
cv::Mat fullMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos,
                cv::Mat imgIsos, float l, bool offset_sections, bool saving);