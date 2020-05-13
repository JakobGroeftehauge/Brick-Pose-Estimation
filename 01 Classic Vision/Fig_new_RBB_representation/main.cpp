#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "../Includes/util.h"
#include "../Includes/annotation_loader.h"

using namespace std;


std::string image_str = "colorIMG_137";

void print_rotated_rect_horisontal(cv::Mat &img, cv::RotatedRect rect, cv::Scalar color = cv::Scalar(0,0,255))
{
    cv::RotatedRect corrected_rect = ensure_bb_orientation(rect);

    cv::RotatedRect rot_rect_horisontal(corrected_rect.center, corrected_rect.size, 0);
    cv::Point2f rect_points[4];
    rot_rect_horisontal.points(rect_points);

    for(int i = 0; i < 4; i++)
    {
        line(img, rect_points[i], rect_points[(i+1)%4], color, 1, CV_AA);
    }

    cv::circle(img, rect_points[1], 3, color, -1,  CV_AA);
    cv::circle(img, rect_points[3], 3, color, -1,  CV_AA);
}

int main()
{
    Annotation_loader annotations;

    annotations.loadAnnotation("../../03 Data/Simple Dataset Copied/"+ image_str+".json");
    cv::Rect roi(cv::Point(225, 70), cv::Point(610, 420));

    cv::Mat img = cv::imread("../../03 Data/Simple Dataset Copied/" + image_str + ".png");
    std::vector<cv::RotatedRect> rot_rects;
    std::vector<cv::Point2f> end_points;

    for (int i = 0; i < annotations.Rect_list.size(); i++)
    {
        rot_rects.push_back(ensure_bb_orientation(cv::minAreaRect(annotations.Rect_list[i])));
        end_points.push_back(cv::Point2f(cos(rot_rects[i].angle/180*CV_PI),sin(rot_rects[i].angle / 180 * CV_PI))*25);

    }


    //util::print_rotated_bounding_boxes(img, rot_rects, cv::Scalar(255,255,0),CV_AA);


    for (int i = 0; i < end_points.size(); i++)
    {
        print_rotated_rect_horisontal(img, rot_rects[i], cv::Scalar(255, 255, 0));
        cv::RotatedRect corrected_rect = ensure_bb_orientation(rot_rects[i]);
        cv::arrowedLine(img, corrected_rect.center-end_points[i],corrected_rect.center+end_points[i], cv::Scalar(255, 255, 0),1,CV_AA);
    }
    cv::Mat img_cropped = img(roi);
    cv::Mat resized(img_cropped);
    cv::resize(resized, resized, resized.size()*2, 0, 0, CV_INTER_CUBIC);

    cv::imshow("rotated bbox", resized);
    imwrite(image_str + "_rot_angle_pred.png", resized);
    cv::waitKey(0);
    return 0;
}
