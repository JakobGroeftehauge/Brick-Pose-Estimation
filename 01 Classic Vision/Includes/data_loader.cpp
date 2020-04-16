#include <string>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include "data_loader.h"
#include "annotation_loader.h"


Data_loader::Data_loader()
{

}

Data_loader::Data_loader(std::string path_to_folder)
{
    this->path_folder = path_to_folder;
    this->file_paths_iterator = 0;
    //std::cout << this->path_folder << std::endl;
    //Store path to all .json files in folder
    for(const auto & entry : std::experimental::filesystem::directory_iterator(path_to_folder))
    {
        std::string load_path = entry.path().string();

        if(ends_with(load_path, ".json") == true)
        {
            //std::cout << load_path << std::endl;
            file_paths.push_back(load_path);
        }
    }

}

Data_loader::Data_loader(std::string path_to_folder, std::string img_list_csv)
{
    this->path_folder = path_to_folder;
    this->file_paths_iterator = 0;
    //std::cout << this->path_folder << std::endl;
    std::ifstream fin;
    std::string word;
    // Open an existing file 
    fin.open(img_list_csv);
    while (std::getline(fin, word))
    {
        //std::cout << word.substr(0, word.find_last_of(".")) + ".json" << std::endl;
        this->file_paths.push_back(path_to_folder + "/" + word.substr(0, word.find_last_of(".")) + ".json");
    }
    fin.close();
    //while (std::getline(fin, word,",") )
    //{

    //}
    //std::getline(s, word, ', ')
    // Load paths from csv file
}

bool Data_loader::loadNext()
{
    if(file_paths_iterator < file_paths.size())
    {
        this->annotation_loader.loadAnnotation(file_paths[file_paths_iterator]);
        this->file_name = this->annotation_loader.image_file_name;
        this->img = cv::imread(this->path_folder + "/" + annotation_loader.image_file_name, cv::IMREAD_COLOR);
        convert_to_bb(annotation_loader.Rect_list);
        file_paths_iterator++;
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<std::vector<double>> Data_loader::get_BB_characteristics()
{
    std::vector<std::vector<double>> output_list;

    for(unsigned int i = 0; i < annotation_loader.Rect_list.size(); i++)
    {
        std::vector<double> BB = {};

        cv::Rect rect = cv::boundingRect(annotation_loader.Rect_list[i]);
        cv::RotatedRect rotated_rect = cv::minAreaRect(annotation_loader.Rect_list[i]);

        BB.push_back(rect.width);
        BB.push_back(rect.height);
        BB.push_back(rotated_rect.size.width);
        BB.push_back(rotated_rect.size.height);

        output_list.push_back(BB);
    }

    return output_list;
}


void Data_loader::convert_to_bb(std::vector<std::vector<cv::Point2f>> point_list)
{
    this->annotations.clear();
    for (int i = 0; i < point_list.size(); i++)
    {
        cv::RotatedRect rot_rect = cv::minAreaRect(point_list[i]);
        this->annotations.push_back(bounding_box(rot_rect));
    }
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
