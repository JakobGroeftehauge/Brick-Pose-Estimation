#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
class Bricks_from_lines_img
{
public:
	Bricks_from_lines_img(cv::Mat src_img, std::vector<std::vector<double>> lines);
	void draw_contours(cv::Mat img);
	void draw_rotated_rect(cv::Mat img);
	void draw_rect(cv::Mat img);
	std::vector<std::vector<cv::Point>> get_contours();


private:
	cv::RNG rng;
	std::vector<std::vector<double>> lines;
	cv::Mat base_img;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::RotatedRect> rotated_rectangles;
	int area_upper;
	int area_lower;
	int arc_length_upper;
	int arc_length_lower;

	void print_line_bw(cv::Mat img, double rho, double theta);

};

