#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
struct shifted_hough {
	cv::Mat hough_space;
	int split_value;
};

class Hough_space
{
public:
	Hough_space();
	Hough_space(cv::Mat edge_img);
	//-------- methods -----------
	void save_to_csv(std::string filename);
	// ------ Not settled on yet --------
	std::vector<std::vector<double>> find_lines();
	double split_angle;


	cv::Mat edge_img;
	cv::Mat hough_matrix;
    cv::Mat hough_rotated;
private:
	int img_height;
	int img_width;
	int hough_height;
	int hough_width;
	int resolution_rho;
	int rho_max;
	float resolution_theta;
	int num_angle;
	float accum_threshold;
	std::vector<float> theta_tick;
	std::vector<int> rho_tick;
	std::vector<std::vector<double>> lines;
	cv::RNG rng;

	//-------------- Methods ---------------------
	cv::Point get_parameters(cv::Point idx_point);
	void initialise_hough_space();
	void fill_ticks();
	int clusters_at_border(cv::Mat hough_space_bin);
	int find_splitting_line(cv::Mat hough_space_bin);
	std::vector<std::vector<cv::Point>> inverse_shift_points(std::vector<std::vector<cv::Point>> contours, int theta_split);
	std::vector<cv::Point2f> inverse_shift_single_contour(std::vector<cv::Point2f> contour, int theta_split);
	cv::Point2f inverse_shift_single_point(cv::Point2f point, int theta_split);
	void show_contours(std::vector<std::vector<cv::Point>> contour_list);
	std::vector<double> convert_to_line(cv::Point pos_hough_space);
	std::vector<double> convert_to_line(cv::Point2f pos_hough_space);
	cv::Point get_maximum(std::vector<cv::Point> contour);
	cv::Point2f get_weighted_average(std::vector<cv::Point> contour, int split_value);
	void shift_hough(cv::Mat hough_space_src, cv::Mat& hough_space_dst, int split_value);

};

