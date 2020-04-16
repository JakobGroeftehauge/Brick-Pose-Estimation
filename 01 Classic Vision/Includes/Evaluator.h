#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "util.h"
#include "data_loader.h"
#include "Detector.h"
#include "Brick_Detector.h"
#include "Chamfer_brick_detector.h"
struct annotation_match {
	annotation_match(int prediction_idx_, int annotation_idx_, double IoU_)
	{
		this->annotation_idx = annotation_idx_;
		this->prediction_idx = prediction_idx_;
		this->IoU = IoU_;
	}

	int annotation_idx;
	int prediction_idx;
	double IoU;
};

class Evaluator
{
public:
	Evaluator();
	Evaluator(std::string path);
	Evaluator(std::string path_to_folder, std::string img_list_csv);
	void set_detector(Detector * detector_addr);
	void set_path(std::string path);
	bool evaluate_next_img();
	void set_thresholds(std::vector<double> thresholds = std::vector<double>({ 0.5 }));
	void open_file();
	void close_file();
	void reset_counters();
	void print_metrics();
	Detector* detector;
	cv::Mat img_to_print;
	std::vector<double> true_positive_range;
	std::vector<double> false_positive_range;
	std::vector<double> false_negative_range;


private:
	double calculate_IoU(cv::Rect rect1, cv::Rect rect2);
	double calculate_IoU(cv::RotatedRect rect1, cv::RotatedRect rect2);
	std::vector<double> evaluate_thresholds;
	//void print_results_to_csv();
	void evaluate_bb(double threshold, int list[3]);
	void evaluate_range(std::vector<double> thresholds);
	void save_evaluation(int true_pos, int false_pos, int false_neg);
	//std::string folder_path;
	void match_annotations(int rect_type);
	std::vector<annotation_match> annotation_matches;
	Data_loader loader;
	std::ofstream file;
    std::vector<cv::Rect> false_positive;
    std::vector<cv::Rect> true_positive;
	float total_true_positive;
	float total_false_positive;
	float total_false_negative;

};

