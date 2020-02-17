#ifndef DATA_LOADER_H
#define DATA_LOADER_H
#include <opencv2/opencv.hpp>
#include <vector>
#include "annotation_loader.h"

class Data_loader
{
public:
    Data_loader(std::string path_to_folder);
    bool Load_Next();
    std::vector<cv::Rect> Bounding_boxes;
    std::vector<double> angle_vector;
    cv::Mat img;

    std::vector<std::string> file_paths;
    unsigned int file_paths_iterator;
    Annotation_loader annotation_loader;
    std::vector<cv::Rect> convert_points_to_rects(std::vector<std::vector<cv::Point2f>> annotation_points);
    std::vector<double> convert_points_to_angles(std::vector<std::vector<cv::Point2f>> annotation_points);
    bool ends_with(const std::string &mainStr, const std::string &toMatch);
};

#endif // DATA_LOADER_H
