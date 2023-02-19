#pragma once
#include "../boiler-plate/invert.hpp"
#include "combine.hpp"
#include "distance-map2.hpp"
#include "offset-map.hpp"

/*
    cv::Mat fullMap(std::string pathEdges, cv::Mat imgEdges, std::string
pathIsos, cv::Mat imgIsos, bool saving);

Given an input image of edges and an
input image of isophotes, generates a distance map from the edges and isophotes,
then returns an offset map with a white background and offset lines of color
corrsponding to the line's id
*/
cv::Mat fullMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos,
                cv::Mat imgIsos, float l, bool saving);