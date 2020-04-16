#ifndef PREDICTIONS_H
#define PREDICTIONS_H
#include <opencv2/opencv.hpp>

inline cv::RotatedRect ensure_bb_orientation(cv::RotatedRect rot_rect)
{
    if (rot_rect.size.width < rot_rect.size.height)
    {
        rot_rect = cv::RotatedRect(rot_rect.center, cv::Size(rot_rect.size.height, rot_rect.size.width), rot_rect.angle + 90);
        if (rot_rect.angle < -90)
        {
            rot_rect.angle += 180;
        }
        else if (rot_rect.angle > 90)
        {
            rot_rect.angle -= 180;
        }
    }
    return rot_rect;
};

struct prediction_candidate {
	cv::RotatedRect rotated_rect;
	float distance_score;
    bool operator < (const prediction_candidate & candidate) 
    { 
        return (distance_score < candidate.distance_score); 
    }
};

struct bounding_box {
    bounding_box(cv::RotatedRect rot_rect_)
    {
        this->rotated_rect = ensure_bb_orientation(rot_rect_);
        this->rect = this->rotated_rect.boundingRect();
        this->angle = this->rotated_rect.angle;
    }
    cv::Rect rect;
    cv::RotatedRect rotated_rect;
    //cv::Point2d angle_vector;
    double angle;
};

#endif // ANNOTATION_LOADER_H
