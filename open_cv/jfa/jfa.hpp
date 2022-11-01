#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "read-save.hpp"

using pixel_type = std::pair<int, int>;
using seed_map = std::unordered_map<pixel_type, pixel_type, hash_pair>;  