#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <opencv2/opencv.hpp>
#include "Brick_Detector.h"


namespace util
{

inline void print_bounding_boxes(cv::Mat& img, std::vector<prediction> preds, cv::Scalar color = cv::Scalar(255,0,0))
{
    for(unsigned int i = 0; i < preds.size(); i++)
    {
        cv::rectangle(img, preds[i].rect.tl(), preds[i].rect.br(), color);
    }
}
inline void print_bounding_boxes(cv::Mat& img, std::vector<cv::Rect> preds, cv::Scalar color = cv::Scalar(255, 0, 0))
{
    for (unsigned int i = 0; i < preds.size(); i++)
    {
        cv::rectangle(img, preds[i].tl(), preds[i].br(), color);
    }
}


inline void print_lines(cv::Mat &img, std::vector<std::vector<double>> lines, cv::Scalar color = cv::Scalar(0, 255, 0))
{
    for(unsigned int i = 0; i < lines.size(); i++)
    {
        cv::Point pt1, pt2;
        double a = cos(lines[i][1]), b = sin(lines[i][1]);
        double x0 = a * lines[i][0], y0 = b * lines[i][0];
        int size = 1000;//max(img.size().height, img.size().width) * 2; //just needs to be big enough.
        pt1.x=cvRound(x0 + size*(-b)) + img.size().width/2;
        pt1.y=cvRound(y0 + size*(a)) + img.size().height/2;
        pt2.x=cvRound(x0 - size*(-b)) + img.size().width/2;
        pt2.y=cvRound(y0 - size*(a)) + img.size().height/2;
        cv::line(img, pt1, pt2, color, 1, cv::LINE_AA);
    }
}

inline void print_rotated_bounding_boxes(cv::Mat &img, std::vector<prediction> preds, cv::Scalar color = cv::Scalar(0, 0, 255))
{
    for(unsigned int  i = 0; i < preds.size(); i++)
    {
        cv::Point2f rect_points[4];
        preds[i].rotated_rect.points(rect_points);

        for(int j = 0; j < 4; j++)
        {
            cv::line(img, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
        }
    }
}




}
#endif // UTIL_H

