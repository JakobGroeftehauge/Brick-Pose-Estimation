#include "annotation_loader.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

Annotation_loader::Annotation_loader()
{

}

void Annotation_loader::loadAnnotation(std::string path)
{
    this->Rect_list.clear();

    std::ifstream file(path);
    json file_obj = json::parse(file);
    this->image_file_name = file_obj["imagePath"];

    // Load points into Rect_list
    json shapes = file_obj["shapes"];
    std::vector<cv::Point2f> polygon;

    for(auto it = shapes.begin(); it != shapes.end(); ++it)
    {
        polygon.clear();
        json &val = it.value();
        std::vector<std::vector<double>> point_list = val["points"];
        for(unsigned int i = 0; i < point_list.size(); i++)
        {
            polygon.push_back(cv::Point2f(point_list[i][0], point_list[i][1]));
        }
        this->Rect_list.push_back(polygon);
    }
}
