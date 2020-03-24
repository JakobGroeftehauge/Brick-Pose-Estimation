#ifndef PREDICTIONS_H
#define PREDICTIONS_H
#include <opencv2/opencv.hpp>

struct prediction_candidate {
	cv::RotatedRect rotated_rect;
	float distance_score;
};


struct prediction {
    cv::Rect rect;
    cv::RotatedRect rotated_rect;
    cv::Point2d angle_vector;
};


#endif // ANNOTATION_LOADER_H
