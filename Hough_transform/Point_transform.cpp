#include "Point_transform.h"

Point_transform::Point_transform(cv::Point2d focal_len)
{
	this->focal_len = focal_len;
	this->transformation_mat = cv::Mat::eye(4,4,CV_32F);
	this->origin_delta = { 0,0 };

}

Point_transform::Point_transform(cv::Point2d focal_len, cv::Point2d origin_delta)
{
	this->focal_len = focal_len;
	this->transformation_mat = cv::Mat::eye(4, 4, CV_32F);
	this->origin_delta = origin_delta;
}

Point_transform::Point_transform(cv::Mat transformation_mat, cv::Point2d focal_len, cv::Point2d origin_delta)
{
	this->focal_len = focal_len;
	this->transformation_mat = transformation_mat;
	this->origin_delta = origin_delta;
}

cv::Point3f Point_transform::transform_point(cv::Point2d point, float depth)
{
	cv::Point3d transformed_point;
	transformed_point.x = (point.x - this->origin_delta.x) * depth / this->focal_len.x; 
	transformed_point.y = (point.y - this->origin_delta.y) * depth / this->focal_len.y; 
	transformed_point.z = depth;
	// ------------------ transformation matrix ----------------------
	transformed_point.x = transformed_point.x * transformation_mat.at<float>(0, 0) + transformed_point.y * transformation_mat.at<float>(0, 1)\
		+ transformed_point.z * transformation_mat.at<float>(0, 2) + 1 * transformation_mat.at<float>(0, 3);
	transformed_point.y = transformed_point.x * transformation_mat.at<float>(1, 0) + transformed_point.y * transformation_mat.at<float>(1, 1)\
		+ transformed_point.z * transformation_mat.at<float>(1, 2) + 1 * transformation_mat.at<float>(1, 3);
	transformed_point.z = transformed_point.x * transformation_mat.at<float>(2, 0) + transformed_point.y * transformation_mat.at<float>(2, 1)\
		+ transformed_point.z * transformation_mat.at<float>(2, 2) + 1 * transformation_mat.at<float>(2, 3);

	return transformed_point;
}
