#ifndef ANNOTATION_FILE_H
#define ANNOTATION_FILE_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "json.hpp"

using  json = nlohmann::json;

struct annotation_file
{
    std::string file_name; //only supports .png
    std::string relative_path = "";
    int img_height = 480;
    int img_width = 640;
    std::vector<std::vector<std::vector<float>>> polygon_list;
};

void create_annotation_file(annotation_file anno)
{
    json file;
    file["version"] = "3.21.1";

    file["flags"] = json::object({});

    for(unsigned int i = 0; i < anno.polygon_list.size(); i++)
    {
        json shape_obj = json::object({});
        shape_obj += {"label", "Brick"};
        shape_obj += {"line_color", {}};
        shape_obj += {"fill_color", {}};
        shape_obj += {"shape_type", "polygon"};
        shape_obj += {"points", anno.polygon_list[i]};
        shape_obj += {"flags", json::object({})};
        file["shapes"] += shape_obj;
    }
    file["lineColor"] = {0 ,255, 0, 128};
    file["fillColor"] = {255, 0, 0, 128};
    file["imageData"] = {};
    file["imagePath"] = anno.file_name;
    file["imageHeight"] = anno.img_height;
    file["imageWidth"] = anno.img_width;

    std::string json_file_name = anno.file_name;
    json_file_name.erase(json_file_name.end()-4, json_file_name.end());
    json_file_name += ".json";
    std::ofstream o(anno.relative_path + json_file_name);
    o  << std::setw(4) << file << std::endl;
}


#endif // ANNOTATION_FILE_H
