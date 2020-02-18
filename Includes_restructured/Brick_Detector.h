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
	Brick_Detector(cv::Mat img);
	void detect();
	void detect(cv::Mat img);
	void set_img(cv::Mat img);
	std::vector<prediction> predictions;
private:
	cv::Mat img;
	void clear();

};

