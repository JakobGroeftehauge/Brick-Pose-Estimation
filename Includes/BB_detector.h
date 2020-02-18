#ifndef BB_DETECTOR_H
#define BB_DETECTOR_H

#include <vector>
#include <opencv2/opencv.hpp>

using namespace  std;
class BB_detector
{
public:
    BB_detector();
    void detect_BB(vector<vector<double>> lines);

    //private:
    vector<cv::Rect> BB;
    vector<cv::RotatedRect> rotated_BB;
    int img_height;
    int img_width;
    vector<vector<vector<double>>> cluster_lines(vector<vector<double>> lines);
    vector<vector<double>> sort_lines(vector<vector<double>> lines);
    vector<cv::Point2f> get_points_on_line(vector<double> line);
    cv::Point2f get_intersection(vector<double> line1, vector<double> line2);
    vector<vector<cv::Point2f>> get_intersection_matrix(vector<vector<vector<double>>> clustered_lines);
    bool point_within_boundaries(cv::Point2f point);
    void get_bounding_boxes(vector<vector<cv::Point2f>> intersection_matrix);
    bool accept_detection(cv::RotatedRect rotated_BB);
};

#endif // BB_DETECTOR_H
