#ifndef BRICK_DETECTOR_H
#define BRICK_DETECTOR_H
#include "Hough_space.h"
#include "data_loader.h"
#include "BB_detector.h"


class Brick_Detector
{
public:
    Brick_Detector(std::string path_to_folder);
    void predict_all_images();
    std::vector<std::vector<double>> get_lines(cv::Mat img);
    void show_predictions(cv::Mat img, std::vector<cv::Rect> BB);
    void show_lines(cv::Mat img, vector<vector<double>> lines);
    void evaluate_predictions(double threhold);
    void save_evaluation(int true_pos, int false_pos, int false_neg);
    Data_loader data_loader;
    BB_detector boundingBox_detector;
    int uppper_thres_canny = 140;
    int lower_thres_canny = 60;

    double intersection_area(cv::Rect rect1, cv::Rect rect2);
    double union_area(cv::Rect rect1, cv::Rect rect2);
    std::ofstream file;

private:
};

#endif // BRICK_DETECTOR_H
