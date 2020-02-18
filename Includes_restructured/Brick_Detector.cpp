#include "Brick_Detector.h"
#include "Hough_space.h"

Brick_Detector::Brick_Detector()
{
}

Brick_Detector::Brick_Detector(cv::Mat img)
{
    this->img = img;
}

void Brick_Detector::detect()
{
    clear_predictions();
    find_lines();


}

void Brick_Detector::detect(cv::Mat img)
{
    set_img(img);
    detect();
}

void Brick_Detector::set_img(cv::Mat img)
{
    this->img = img;
}

void Brick_Detector::clear_all()
{
    this->img.release();
    clear_predictions();
}

void Brick_Detector::clear_predictions()
{
    this->predictions = {};
}

void Brick_Detector::find_lines()
{
    cv::Mat edge_img;
    find_edges(this->img, edge_img);

    Hough_space hough(edge_img);

    this->lines =  hough.find_lines();
}

void Brick_Detector::find_edges(cv::Mat &src, cv::Mat &dst)
{
    cv::Mat gray_img;
    cv::cvtColor(src, gray_img, CV_BGR2GRAY);

    cv::Mat filter_img;
    cv::medianBlur(gray_img, filter_img, 5);

    cv::Canny(filter_img, src, this->canny_thres_low, this->canny_thres_high);
}

void Brick_Detector::find_BB()
{
    std::vector<std::vector<std::vector<double>>> clustered_lines;
    std::vector<std::vector<std::vector<double>>> sorted_lines;
    std::vector<std::vector<cv::Point2f>> intersection_matrix;

    clustered_lines = cluster_lines();

    if(clustered_lines[0].size() > 0 && clustered_lines[1].size() > 0)
    {
        sorted_lines = {sort_lines(clustered_lines[0]), sort_lines(clustered_lines[1])};
        intersection_matrix = find_intersection_matrix(sorted_lines);
    }
}



#include "Brick_Detector.h"
