#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "data_loader.h"
#include "Brick_Detector.h"
#include "util.h"
class Evaluator
{
public:
	Evaluator();
	Evaluator(std::string path);
	void set_path(std::string path);
	bool evaluate_next_img(double threshold = 0.5);
	void open_file();
	void close_file();
	cv::Mat img_to_print;

private:
	double calculate_IoU(cv::Rect rect1, cv::Rect rect2);
	//void print_results_to_csv();
	void evaluate(double threshold);
	void save_evaluation(int true_pos, int false_pos, int false_neg);
	//std::string folder_path;
	Data_loader loader;
	std::ofstream file;
	Brick_Detector detector;
};

