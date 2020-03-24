#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "predictions.h"
#include <math.h>
#include <chrono>

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
	cv::Mat model_template;
	cv::Mat matching_space;
	std::vector<cv::RotatedRect> best_rects;
	std::vector<prediction_candidate> pred_candidates;

private:
    cv::Mat img;
	void find_edges(cv::Mat& src, cv::Mat& dst);
	void apply_NMS(std::vector<cv::Point>& dst_locations, float thresh);
	void compute_chamfer_img();
	cv::RotatedRect create_template(float scale, float angle);
	void create_rot_rect_at_locations(std::vector<cv::Point> &locations, cv::RotatedRect template_rect);
    
	int canny_thres_low;
    int canny_thres_high;


    void clear_all();
    void clear_predictions();
    bool accept_detection(cv::RotatedRect rotated_BB);

};

