#include "Chamfer_brick_detector.h"

Chamfer_brick_detector::Chamfer_brick_detector()
{
	this->canny_thres_high = 140;
	this->canny_thres_low = 100;
}

Chamfer_brick_detector::Chamfer_brick_detector(cv::Mat img)
{
	Chamfer_brick_detector();
	this->img = img;
	compute_chamfer_img();
}

void Chamfer_brick_detector::set_img(cv::Mat img)
{
	this->img = img;
}

void Chamfer_brick_detector::compute_chamfer_img()
{
	cv::Mat edge_img;
	find_edges(this->img, edge_img);
	this->chamfer_img = edge_img;
	//cv::threshold(edge_img, edge_img, 127, 255, CV_THRESH_BINARY_INV);

	//cv::distanceTransform(edge_img, this->chamfer_img, CV_DIST_L2, 3);
}

void Chamfer_brick_detector::find_edges(cv::Mat& src, cv::Mat& dst)
{
	cv::Mat gray_img;
	cv::cvtColor(src, gray_img, CV_BGR2GRAY);

	cv::Mat filter_img;
	cv::medianBlur(gray_img, filter_img, 5);

	cv::Canny(filter_img, dst, this->canny_thres_low, this->canny_thres_high);
}
