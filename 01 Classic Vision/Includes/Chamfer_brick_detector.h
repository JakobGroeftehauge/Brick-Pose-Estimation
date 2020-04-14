#pragma once
#include "Detector.h"
#include <math.h>

class Chamfer_brick_detector: public Detector
{
public:
	Chamfer_brick_detector();
	Chamfer_brick_detector(cv::Mat img);
	void detect();
	void detect(cv::Mat img);
	void compute_chamfer_img();
	void set_NMS_thresh(double thresh);
    void set_resolution(int angle_step, float scale_min, float scale_max, float scale_step);
	cv::Mat chamfer_img;
	cv::Mat model_template;
	cv::Mat matching_space;
    cv::Mat matching_space_disp;
	std::vector<cv::RotatedRect> best_rects;
	std::vector<prediction_candidate> pred_candidates;
    std::vector<prediction_candidate> pred_candidates_unfiltered;
	void create_template(float scale, float angle, cv::Mat& template_img_dst, cv::RotatedRect& rect_dst);


private:
	void find_edges(cv::Mat& src, cv::Mat& dst);
	void apply_NMS(cv::Mat& matching_space_src, std::vector<cv::Point> &best_match_locations);
	void find_rectangle_candidates(int angle_steps, float scale_min, float scale_max, int scale_steps);
	void generate_candidates(std::vector<cv::Point> &best_match_locations, cv::Mat &matching_space, cv::RotatedRect template_rect, std::vector<prediction_candidate>& candidates_dst);
	void apply_IOU_NMS(const std::vector<prediction_candidate>& candidates_src, float thresh, std::vector<prediction_candidate>& candidates_dst);
	float rotated_rect_IOU(cv::RotatedRect rect1, cv::RotatedRect rect2);
	void predictions_from_candidates(std::vector<prediction_candidate> &candidates, std::vector<prediction> &predictions);

    double NMS_thresh;

    int angle_step = 90;
    float scale_min = 100.0;
	float scale_max = 135.0;
    float scale_step = 10;

    int template_padding = 2;
	cv::RotatedRect brick_template = cv::RotatedRect(cv::Point2f(0.5, 0.5), cv::Size2f(0.971681, 0.236297), 0);

};
