#ifndef ANNOTATION_LOADER_H
#define ANNOTATION_LOADER_H
#include <string>
#include <opencv2/opencv.hpp>


class Annotation_loader
{
public:
    Annotation_loader();
    void LoadAnnotation(std::string path);
    std::string imagePath;
    std::vector<std::vector<cv::Point2f>> Rect_list;
};

#endif // ANNOTATION_LOADER_H
