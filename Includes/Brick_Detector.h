#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "Hough_space.h"
struct prediction {
	cv::Rect rect;
	cv::RotatedRect rotated_rect;
	cv::Point2d angle_vector;
};

class Brick_Detector
{

public:
	Brick_Detector();
	Brick_Detector(cv::Mat img);
	void detect();
	void detect(cv::Mat img);
	void set_img(cv::Mat img);
	std::vector<prediction> predictions;
    std::vector<std::vector<double>> lines;

private:
	cv::Mat img;

    int canny_thres_low;
    int canny_thres_high;

    void clear_all();
    void clear_predictions();
    void find_lines();
    void find_BB(std::vector<std::vector<std::vector<double>>> clustered_lines);
    void find_edges(cv::Mat &src, cv::Mat &dst);

    std::vector<std::vector<std::vector<double>>> cluster_lines();
    std::vector<std::vector<double>> sort_lines(std::vector<std::vector<double>> lines);
    std::vector<std::vector<cv::Point2f>> find_intersection_matrix(std::vector<std::vector<std::vector<double>>> clustered_lines_sorted);
    cv::Point2f find_intersection(std::vector<double> line1, std::vector<double> line2);
    std::vector<cv::Point2f> get_points_on_line(std::vector<double> line);
    void convert_intections_to_BB(std::vector<std::vector<cv::Point2f>> intersection_matrix);
    bool accept_detection(cv::RotatedRect rotated_BB);
};

