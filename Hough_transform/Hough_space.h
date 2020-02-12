#pragma once
#include <opencv2/opencv.hpp>
class Hough_space
{
public:
	Hough_space(cv::Mat edge_img);

private:
	cv::Mat edge_img;
	cv::Mat hough_space;
	int img_height;
	int img_width;
	int hough_height;
	int hough_width;
	int resolution_rho;
	int rho_max;
	float resolution_theta;
	int num_angle;




};

