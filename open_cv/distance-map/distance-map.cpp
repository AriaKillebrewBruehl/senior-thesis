#include "distance-map.hpp"

distMap distanceMap(std::string pathEdges, cv::Mat imgEdges, std::string pathIsos, cv::Mat imgIsos, bool saving) {
    // read images and resize
    cv::Mat edges;
    edges = read(pathEdges, imgEdges);
     try {
        if (edges.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in distanceMap." << std::endl;
        distMap dMap;
        return dMap;
    }
    if (edges.type() == 0) {
        cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
    }
    resz(edges);
    cv::Mat isos;
    isos = read(pathIsos, imgIsos);
     try {
        if (isos.empty()) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Could not read in image in distanceMap." << std::endl;
        distMap dMap;
        return dMap;
    }
    if (isos.type() == 0) {
        cv::cvtColor(isos, isos, cv::COLOR_GRAY2BGR);
    }
    resz(isos);
    
    seed_map edgeJfa = jmp_flood_seeds("", edges);
    seed_map isosJfa = jmp_flood_seeds("", isos);

    cv::Mat distances = cv::Mat::zeros(edges.rows, edges.cols, CV_8UC1);
    cv::Mat priorityBuffer = cv::Mat::zeros(edges.rows, edges.cols, CV_8UC1);

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
                if (isos_dist > 255) {
                    isos_dist = 255;
                }
                distances.at<uchar>(i, j) = uchar(isos_dist);
                
                priorityBuffer.at<uchar>(i, j) = uchar(isos_weight);
            } else {
                if (edge_dist > 255) {
                    edge_dist = 255;
                }
                distances.at<uchar>(i, j) = uchar(edge_dist);
               
                priorityBuffer.at<uchar>(i, j) = uchar(edge_weight);
            }
        }
    }
    // save image
    if (saving) {
        save(distances, pathEdges, "-dists?");
        save(priorityBuffer, pathEdges, "-priorities");
    }

    distMap dists;
    dists.distances = distances;
    dists.priorityBuffer = priorityBuffer;

    return dists;
}

// int main(int argc, char** argv) {
//     if (argc < 2) {
//          std::cerr << "Must pass in image to run jfa on." << std::endl;
//     } else {
//        for (int i = 1; i < argc; i += 2) {
//             cv::Mat image;
//             distanceMap(argv[i], image, argv[i+1], image, true);
//        }
//     }
// }