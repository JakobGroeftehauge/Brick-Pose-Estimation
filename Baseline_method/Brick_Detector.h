#pragma once
#include <opencv2/opencv.hpp>
struct prediction {
	cv::Rect rect;
	cv::RotatedRect rotated_rect;
	cv::Point2d angle_vector;
};
class Brick_Detector
{
public:
	Brick_Detector();
	void detect();
	void detect(cv::Mat img);

private:
	cv::Mat img;


};

