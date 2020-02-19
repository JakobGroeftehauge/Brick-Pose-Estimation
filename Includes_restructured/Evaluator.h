#pragma once
#include <opencv2/opencv.hpp>
#include "data_loader.h"
#include "Brick_Detector.h"
class Evaluator
{
public:
	Evaluator();
	Evaluator(std::string path);
	void set_path(std::string path);
	void evaluate_next_img();
	void open_file(std::string file_name);
	void close_file();

private:
	double calculate_IoU(cv::Rect rect1, cv::Rect rect2);
	void print_results_to_csv();
	//std::string folder_path;
	Data_loader loader;
	std::ofstream file;
	Brick_Detector detector;
};

