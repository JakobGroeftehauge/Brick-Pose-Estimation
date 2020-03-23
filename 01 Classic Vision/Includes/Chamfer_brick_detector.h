#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "predictions.h"

class Chamfer_brick_detector
{
public:
	Chamfer_brick_detector();
	Chamfer_brick_detector(cv::Mat img);
	void detect();
	void detect(cv::Mat img);
	void set_img(cv::Mat img);
	std::vector<prediction> predictions;
	cv::Mat chamfer_img;

private:
    cv::Mat img;

	void compute_chamfer_img();

    int canny_thres_low;
    int canny_thres_high;

    void clear_all();
    void clear_predictions();
    void find_edges(cv::Mat& src, cv::Mat& dst);

    bool accept_detection(cv::RotatedRect rotated_BB);


};

