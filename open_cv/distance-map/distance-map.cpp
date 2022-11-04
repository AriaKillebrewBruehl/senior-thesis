#include "distance-map.hpp"

cv::Mat distanceMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos, cv::Mat imgIsos, bool saving) {
    // read images and resize
    cv::Mat edges;
    edges = read(pathEdges, imgEdges);
    resz(edges);
    cv::Mat isos;
    isos = read(pathIsos, imgIsos);
    resz(isos);
    
    seed_map edgeJfa = jmp_flood_seeds("", edges);
    seed_map isosJfa = jmp_flood_seeds("", isos);

    cv::Mat distances(edges.rows, edges.cols, CV_8UC3);

    int edge_weight = 1;
    int isos_weight = 2;
    for (int i = 0; i < distances.rows; i++) {
        for (int j = 0; j < distances.cols; j++) {
            pixel_type pix(i, j);
            pixel_type edge_seed = edgeJfa[pix];
            pixel_type isos_seed = isosJfa[pix];
    
            float edge_dist = std::hypot(pix.first - edge_seed.first, pix.second - edge_seed.second) / edge_weight;
            float isos_dist = std::hypot(pix.first - isos_seed.first, pix.second - isos_seed.second) / isos_weight;
            if (isos_dist < edge_dist) {
                distances.at<cv::Vec3b>(i, j)[0] = int(isos_dist);
                distances.at<cv::Vec3b>(i, j)[1] = int(isos_dist);
                distances.at<cv::Vec3b>(i, j)[2] = int(isos_dist);
            } else {
                distances.at<cv::Vec3b>(i, j)[0] = int(edge_dist);
                distances.at<cv::Vec3b>(i, j)[1] = int(edge_dist);
                distances.at<cv::Vec3b>(i, j)[2] = int(edge_dist);
            }
        }
    }
    // save image
    if (saving) {
        save(distances, pathEdges, "-dists");
    }
    return distances;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run jfa on." << std::endl;
    } else {
       for (int i = 1; i < argc; i += 2) {
            cv::Mat image;
            distanceMap(argv[i], image, argv[i+1], image, true);
       }
    }
}