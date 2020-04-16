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
	void set_canny_thresh(int low, int high);
	void set_img(cv::Mat img);
	std::vector<bounding_box> predictions;
	double time = 0;

protected:
	cv::Mat img;
	int canny_thres_low;
	int canny_thres_high;
	void clear_predictions();


};

