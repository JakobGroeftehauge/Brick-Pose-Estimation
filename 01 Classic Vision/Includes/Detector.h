#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "predictions.h"
#include "util.h"

class Detector
{
public:
	Detector();
	Detector(cv::Mat img);
	virtual void detect();
	virtual void detect(cv::Mat img);
	virtual void set_NMS_thresh(double thresh); //remove after use
	virtual void set_brick_specs(double min_w, double min_h, double max_w, double max_h);
	virtual void set_resolution(int angle_step, float scale_step);
	void set_canny_thresh(int low, int high);
	void set_img(cv::Mat img);
	std::vector<bounding_box> predictions;
	void reset_time();
	double time = 0;

protected:
	cv::Mat img;
	int canny_thres_low;
	int canny_thres_high;
	void clear_predictions();
	double max_width, max_height, min_width, min_height;


};

