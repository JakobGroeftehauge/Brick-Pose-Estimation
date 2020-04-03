#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "Detector.h"
#include "Hough_space.h"

class Brick_Detector: public Detector
{

public:
	Brick_Detector();
	Brick_Detector(cv::Mat img);
	void detect();
	void detect(cv::Mat img);
    //void set_NMS_thresh(double thresh); // not used. Only added to support the one in the other detector
    std::vector<std::vector<double>> lines;

private:
    Hough_space hough;
    int canny_thres_low;
    int canny_thres_high;

    void clear_all();
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

