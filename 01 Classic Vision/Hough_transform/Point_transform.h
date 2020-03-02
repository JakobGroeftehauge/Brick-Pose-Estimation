#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
class Point_transform
{
public:
	Point_transform(cv::Point2d focal_len);
	Point_transform(cv::Point2d focal_len, cv::Point2d origin_delta);
	Point_transform(cv::Mat transformation_mat, cv::Point2d focal_len, cv::Point2d origin_delta);
	cv::Point3f transform_point(cv::Point2d point, float depth);

private:
	cv::Mat transformation_mat;
	cv::Point2d focal_len; // millimeters
	cv::Point2d origin_delta;
	
};

