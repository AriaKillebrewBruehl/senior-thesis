#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "../distance-map/full-map.hpp"
#include "../dots/dots.hpp"
#include "../dots/place-dots.hpp"
#include "../edge-detection/extract-edges.hpp"
#include "../isophotes/extract-isophotes.hpp"

using namespace cv;

// set up variables for mouseHandler
Mat mouse_src, img1, mask, mask_outline, final;
Point point;
std::vector<Point> pts;
std::vector<std::vector<Point>> sections;
int drag = 0;
int var = 0;
int flag = 0;
void mouseHandler(int, int, int, int, void *);

// set up default parameter values
const int EDGE_THRESH = 300;
const int BINS = 5;
const int ISOS_HIGHLIGHT_THRESH = 2;
const int ISOS_THRESH = 200;
const int L = 12.0;
const int NEGATIVE_SPACE_BINS = 5;
const int NEGATIVE_SPACE_THRESH = 1;
const int MAX_SIZE = 15;
const int OUTLINE_THRESH = 800;
// setup variables
Mat image;
bool auto_save = false;
Mat edges;
int thresh_edges = EDGE_THRESH;
Mat posterized;
int bins = BINS;
Mat isophotes;
int thresh_iso_highlights = ISOS_HIGHLIGHT_THRESH;
Mat isophotes_extracted;
int thresh_isophotes = ISOS_THRESH;
Mat details;
Mat distances;
Mat offset_map;
Mat offset_map_visual;
int l = L;
Mat initial_dots;
int d = l;
Mat adjusted_dots;
Mat negative_posterized;
int negative_bins = NEGATIVE_SPACE_BINS;
Mat negative_space;
int thresh_negative_space = NEGATIVE_SPACE_THRESH;
Mat rendered;
int max_size = MAX_SIZE;
Mat outline;
int thresh_outline = OUTLINE_THRESH;
Mat additional_details;
int scale = 6;
Mat enlarged_outline;
Mat enlarged_details;
Mat final_rendering;

Mat caboodle(std::string path, Mat img, bool saving);