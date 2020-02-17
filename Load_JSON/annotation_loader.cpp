#include "annotation_loader.h"
#include <iostream>

Annotation_loader::Annotation_loader()
{

}

void Annotation_loader::LoadAnnotation(std::string path)
{
    cv::FileStorage file(path, cv::FileStorage::READ);
    cv::FileNode root = file["shapes"];
    std::vector<std::vector<cv::Point2f>> annotation_point_list;

    for(unsigned int i = 0; i < root.size(); i++)
    {
        std::vector<cv::Point2f> annotation_points;

        cv::FileNode Annotation = root[i]["points"];
        for(unsigned int j = 0; j < Annotation.size();  j++)
        {
            annotation_points.push_back(cv::Point2f(Annotation[j][0].real(), Annotation[j][1].real()));
        }
        annotation_point_list.push_back(annotation_points);
        annotation_points.empty();
    }
    Rect_list = annotation_point_list;

    cv::FileNode img_path   = file["imagePath"];
    imagePath = img_path.string();
    std::cout << imagePath << std::endl;
}
