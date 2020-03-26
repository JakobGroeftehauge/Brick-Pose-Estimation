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
	cv::Mat create_matchingspace(int num_angles, float scale);


private:
	cv::Mat img;
	void find_edges(cv::Mat& src, cv::Mat& dst);
	void apply_NMS(cv::Mat& matching_space_src, std::vector<cv::Point> &best_match_locations);
	void compute_chamfer_img();
	void create_template(float scale, float angle, cv::Mat &template_img_dst, cv::RotatedRect &rect_dst);
	void find_rectangle_candidates(int angle_steps, float scale_min, float scale_max, int scale_steps);
	void generate_candidates(std::vector<cv::Point> &best_match_locations, cv::Mat &matching_space, cv::RotatedRect template_rect, std::vector<prediction_candidate>& candidates_dst);

	int canny_thres_low;
	int canny_thres_high;
	float NMS_thresh;


	void clear_all();
	void clear_predictions();
	bool accept_detection(cv::RotatedRect rotated_BB);

	void set_macth_template();
	int template_padding = 0; //How many pixels to add to the circumfrence of the template Mat.
	 // OBS: size passer ikke! Skal lige regnes igen.
	cv::RotatedRect brick_template = cv::RotatedRect(cv::Point2f(0.5, 0.5), cv::Size2f(0.971681, 0.236297), 0);

};
