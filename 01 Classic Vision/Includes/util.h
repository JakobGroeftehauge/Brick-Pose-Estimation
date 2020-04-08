#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>
#include "predictions.h"

namespace util
{
struct Timer
{
    //std::chrono::time_point<std::chrono::steady_clock> start;
    //std::chrono::time_point<std::chrono::steady_clock> end;
    std::chrono::high_resolution_clock::time_point end;
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::duration<float> duration;
    std::string timer_name;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    Timer(std::string name)
    {
        timer_name = name;
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        std::cout << "Timer \"" + timer_name + "\" took: " << ms << "ms" << std::endl;
    }
};

inline void draw_points(cv::Mat& img, std::vector<cv::Point2f> pt_list, int radius, cv::Scalar color)
{
    for (int j = 0; j < pt_list.size(); j++)
    {
        cv::circle(img, pt_list[j], radius, color, -1, CV_AA);
    }
}

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
        cv::rectangle(img, preds[i].tl(), preds[i].br(), color,1);
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

inline void print_rotated_bounding_boxes(cv::Mat& img, std::vector<cv::RotatedRect> preds, cv::Scalar color = cv::Scalar(0, 0, 255), int line_type = 8)
{
    for (unsigned int i = 0; i < preds.size(); i++)
    {
        cv::Point2f rect_points[4];
        preds[i].points(rect_points);

        for (int j = 0; j < 4; j++)
        {
            cv::line(img, rect_points[j], rect_points[(j + 1) % 4], color, 1, line_type);
        }
    }
}

inline void print_rotated_bounding_boxes(cv::Mat& img, std::vector<prediction_candidate> preds, cv::Scalar color = cv::Scalar(0, 0, 255))
{
    for (unsigned int i = 0; i < preds.size(); i++)
    {
        cv::Point center = preds[i].rotated_rect.center;
        cv::Point2f rect_points[4];
        preds[i].rotated_rect.points(rect_points);

        for (int j = 0; j < 4; j++)
        {
            cv::line(img, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
        }
        cv::putText(img, std::to_string((int)preds[i].distance_score), center - cv::Point(7,-7), 1, 1, cv::Scalar(255, 255, 255));
    }
}

inline void print_filled_rotated_bounding_boxes(cv::Mat &img, std::vector<prediction> preds, float alpha = 0.5, cv::Scalar color = cv::Scalar(0, 0, 255))
{
    cv::Mat img_copy = img.clone();
    for(unsigned int  i = 0; i < preds.size(); i++)
    {
        cv::Point2f f_points[4];
        preds[i].rotated_rect.points(f_points);
        std::vector<cv::Point2f> f_points_vec(f_points, f_points + sizeof f_points / sizeof f_points[0]);
        std::vector<cv::Point2i> points;

        cv::Mat(f_points_vec).convertTo(points, cv::Mat(points).type());
        cv::fillConvexPoly(img_copy, points, color, CV_AA);
    }
    cv::addWeighted(img, 1-alpha, img_copy, alpha, 0.0, img);
}


}
#endif // UTIL_H

