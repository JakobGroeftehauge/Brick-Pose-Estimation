#ifndef DATA_LOADER_H
#define DATA_LOADER_H
#include <opencv2/opencv.hpp>
#include <vector>
#include "annotation_loader.h"

class Data_loader
{
public:
    Data_loader();
    Data_loader(std::string path_to_folder);
    bool loadNext();
    std::vector<cv::Rect> Bounding_boxes;
    std::vector<double> angle_vector;
    cv::Mat img;
    std::string file_name;

private:
    std::vector<std::string> file_paths;
    std::string path_folder;
    unsigned int file_paths_iterator;
    Annotation_loader annotation_loader;
    std::vector<cv::Rect> convert_points_to_rects(std::vector<std::vector<cv::Point2f>> annotation_points);
    std::vector<double> convert_points_to_angles(std::vector<std::vector<cv::Point2f>> annotation_points);
    bool ends_with(const std::string &mainStr, const std::string &toMatch);
};

#endif // DATA_LOADER_H
