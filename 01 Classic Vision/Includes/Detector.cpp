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

void Detector::set_img(cv::Mat img)
{
	this->img = img;
}

void Detector::clear_predictions()
{
	this->predictions.clear();
}
