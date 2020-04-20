#include "Detector.h"

Detector::Detector()
{
}

Detector::Detector(cv::Mat img)
{
	this->img = img;
}

void Detector::detect()
{
}

void Detector::detect(cv::Mat img)
{
}

void Detector::set_NMS_thresh(double thresh)
{
}

void Detector::set_brick_specs(double min_w, double min_h, double max_w, double max_h)
{
	this->min_width = min_w;
	this->min_height = min_h;
	this->max_width = max_w;
	this->max_height = max_h;
}

void Detector::set_canny_thresh(int low, int high)
{
	this->canny_thres_low = low;
	this->canny_thres_high = high;
}

void Detector::set_img(cv::Mat img)
{
	this->img = img;
}

void Detector::reset_time()
{
	this->time = 0;
}

void Detector::clear_predictions()
{
	this->predictions.clear();
}
