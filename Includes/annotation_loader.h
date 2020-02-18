#ifndef ANNOTATION_LOADER_H
#define ANNOTATION_LOADER_H
#include <string>
#include <opencv2/opencv.hpp>
#include <regex>


class Annotation_Loader
{
public:
    Annotation_Loader();
    void loadAnnotation(std::string path);
    std::string imagePath;
    std::vector<std::vector<cv::Point2f>> Rect_list;

private:
    void replace_null(std::string path);
};

#endif // ANNOTATION_LOADER_H
