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
	void set_img(cv::Mat img);
	std::vector<prediction> predictions;

protected:
	cv::Mat img;
	void clear_predictions();

};

