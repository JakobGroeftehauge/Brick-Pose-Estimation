#pragma once
#include <opencv2/opencv.hpp>
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
private:
	cv::Mat img;

    int canny_thres_low;
    int canny_thres_high;

    std::vector<std::vector<double>> lines;

    void clear_all();
    void clear_predictions();
    void find_lines();
    void find_edges(cv::Mat &src, cv::Mat &dst);
    void find_BB();
    std::vector<std::vector<std::vector<double>>> cluster_lines();
    std::vector<std::vector<double>> sort_lines(std::vector<std::vector<double>> lines);
    std::vector<std::vector<cv::Point2f>> find_intersection_matrix(std::vector<std::vector<std::vector<double>>> sorted_lines);

};

