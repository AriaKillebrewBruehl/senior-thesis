#include <math.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "distance-map2.hpp"

/*
    cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, bool saving,
                  bool sections);
    If lines == true, returns an offset map with a white background and offset
    lines of color corrsponding to the line's id. If lines == false, returns
    an offset map with black offset lines and sections of color corresponding to
    the section id.
*/
cv::Mat offsetMap(std::string pathDists, cv::Mat imgDists, float l, bool saving,
                  bool sections);