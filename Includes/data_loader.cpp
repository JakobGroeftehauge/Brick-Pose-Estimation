#include <string>
#include <iostream>
#include <experimental/filesystem>
#include "data_loader.h"



Data_loader::Data_loader()
{

}

Data_loader::Data_loader(std::string path_to_folder)
{
    this->path_folder = path_to_folder;
    this->file_paths_iterator = 0;
    std::cout << this->path_folder << std::endl;
    //Store path to all .json files in folder
    for(const auto & entry : std::experimental::filesystem::directory_iterator(path_to_folder))
    {
        std::string load_path = entry.path().string();

        if(ends_with(load_path, ".json") == true)
        {
            file_paths.push_back(load_path);
        }
    }

}

bool Data_loader::loadNext()
{
    if(file_paths_iterator < file_paths.size())
    {
        this->annotation_loader.loadAnnotation(file_paths[file_paths_iterator]);
        //std::cout << "file path" << this->path_folder + "/" + annotation_loader.imagePath << std::endl;
        this->image_path = this->annotation_loader.imagePath;
        this->img = cv::imread(this->path_folder + "/" + annotation_loader.imagePath, cv::IMREAD_COLOR);
        this->Bounding_boxes = convert_points_to_rects(annotation_loader.Rect_list);
        this->angle_vector = convert_points_to_angles(annotation_loader.Rect_list);
        file_paths_iterator++;
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<cv::Rect> Data_loader::convert_points_to_rects(std::vector<std::vector<cv::Point2f> > annotation_points)
{
    std::vector<cv::Rect> BB_list;
    for(unsigned int i = 0; i < annotation_points.size(); i++)
    {
        BB_list.push_back(cv::boundingRect(annotation_points[i]));
    }

    return BB_list;
}

std::vector<double> Data_loader::convert_points_to_angles(std::vector<std::vector<cv::Point2f> > annotation_points)
{
    std::vector<double> angle_vec;
    //Needs to be done
    for(unsigned int i = 0; i < annotation_points.size(); i++)
    {

    }
    return angle_vec;
}

bool Data_loader::ends_with(const std::string &mainStr, const std::string &toMatch)
{
    if(mainStr.size() >= toMatch.size() && mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}
