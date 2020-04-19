#ifndef DATA_LOADER_H
#define DATA_LOADER_H
#include <opencv2/opencv.hpp>
#include <vector>
#include "annotation_loader.h"
#include "predictions.h"

class Data_loader
{
public:
    Data_loader();
    Data_loader(std::string path_to_folder);
    Data_loader(std::string path_to_folder, std::string img_list_csv);
    bool loadNext();
    std::vector<std::vector<double>> get_BB_characteristics(); //Used for determine threshols for filtering bounding boxes.
    //std::vector<cv::Rect> Bounding_boxes;
    //std::vector<double> angle_vector;
    std::vector<bounding_box> annotations;
    cv::Mat img;
    std::string file_name;
    std::string path_folder;

private:
    std::vector<std::string> file_paths;
    unsigned int file_paths_iterator;
    Annotation_loader annotation_loader;
    void convert_to_bb(std::vector<std::vector<cv::Point2f>> point_list);
    bool ends_with(const std::string &mainStr, const std::string &toMatch);
};

#endif // DATA_LOADER_H
