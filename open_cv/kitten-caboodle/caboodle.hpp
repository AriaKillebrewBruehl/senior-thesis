#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "../distance-map/fullMap.hpp"
#include "../dots/dots.hpp"
#include "../dots/place-dots.hpp"
#include "../edge-detection/extract-edges.hpp"
#include "../isophotes/extract-isophotes.hpp"

// set up default parameter values
const int EDGE_THRESH = 300;
const int ISOS_HIGHLIGHT_THRESH = 1;
const int ISOS_THRESH = 200;
const int L = 6.0;
const int NEGATIVE_SPACE_THRESH = 1;
const int MAX_SIZE = 15;
const int OUTLINE_THRESH = 800;
// setup variables
bool auto_save = false;
cv::Mat edges;
int thresh_edges = EDGE_THRESH;
cv::Mat isophotes;
int thresh_iso_highlights = ISOS_HIGHLIGHT_THRESH;
cv::Mat isophotes_extracted;
int thresh_isophotes = ISOS_THRESH;
cv::Mat distances;
cv::Mat offset_map;
cv::Mat offset_map_visual;
int l = L;
cv::Mat initial_dots;
int d = l;
cv::Mat adjusted_dots;
cv::Mat negative_space;
int thresh_negative_space = NEGATIVE_SPACE_THRESH;
cv::Mat rendered;
int max_size = MAX_SIZE;
cv::Mat outline;
int thresh_outline = OUTLINE_THRESH;
cv::Mat final_rendering;

cv::Mat caboodle(std::string path, cv::Mat img, bool saving);