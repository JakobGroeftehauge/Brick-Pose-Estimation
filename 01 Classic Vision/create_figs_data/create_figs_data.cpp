#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
//#include "predictions.h"
#include "../Includes/util.h"
#include "../Includes/annotation_loader.h"

std::string image_str = "colorIMG_193";

int main()
{
    Annotation_loader annotations;
    annotations.loadAnnotation("../../03 Data/Simple Dataset Copied/"+ image_str+".json");

    // ------------ PRINT RAW ANNOTATIONS --------------------------------------
    cv::Mat img = cv::imread("../../03 Data/Simple Dataset Copied/" + image_str + ".png");
    cv::Mat fills = img.clone();
    int pt_radius = 3;
    cv::Scalar dot_color(255, 255, 0), line_color(255, 255, 0), fill_color(255, 255, 0);
    for (int i = 0; i < annotations.Rect_list.size(); i++)
    {
        std::vector<cv::Point2i> points; 
        cv::Mat(annotations.Rect_list[i]).convertTo(points, cv::Mat(points).type()); // convert to point2i as point2f seems to chrash things
        cv::fillConvexPoly(fills, points, fill_color, 8);
    }
    cv::addWeighted(img, 0.7, fills, 0.3, 0.0, img);
    for (int i = 0; i < annotations.Rect_list.size(); i++)
    {
        util::draw_points(img, annotations.Rect_list[i], pt_radius, dot_color);
        for (int j = 0; j < annotations.Rect_list[i].size(); j++)
        {
            cv::line(img, annotations.Rect_list[i][j], annotations.Rect_list[i][(j + 1) % annotations.Rect_list[i].size()], line_color, 1, CV_AA);
        }
    }
    cv::imshow("img", img);
    cv::Rect roi(cv::Point(210, 110), cv::Point(595, 460));
    cv::Mat img_cropped = img(roi);
    cv::imshow("img", img_cropped);
    imwrite(image_str + "_annotated.png", img_cropped);
    //-------------------------------------------------------------------------------------------

    // ------------------------- PRINT ROTATED RECT AND ANGLE -----------------------------------
    img = cv::imread("../../03 Data/Simple Dataset Copied/" + image_str + ".png");
    std::vector<cv::RotatedRect> rot_rects;
    std::vector<cv::Point2f> end_points;
    for (int i = 0; i < annotations.Rect_list.size(); i++)
    {
        rot_rects.push_back(cv::minAreaRect(annotations.Rect_list[i]));
        end_points.push_back(cv::Point2f(cos(rot_rects[i].angle/180*CV_PI),sin(rot_rects[i].angle / 180 * CV_PI))*25);
    }
    util::print_rotated_bounding_boxes(img, rot_rects, cv::Scalar(255,255,0),CV_AA);
    for (int i = 0; i < end_points.size(); i++)
    {
        cv::arrowedLine(img, rot_rects[i].center-end_points[i], rot_rects[i].center+end_points[i], cv::Scalar(255, 255, 0),1,CV_AA);
    }
    img_cropped = img(roi);
    cv::Mat resized(img_cropped);
    cv::resize(resized, resized, resized.size()*2, 0, 0, CV_INTER_CUBIC);
    cv::imshow("rotated bbox", resized);
    imwrite(image_str + "_angle.png", resized);
    // --------------------------------------------------------------------------------------------

    // --------------------------- PRINT AXIS-ALIGNED BBOX ---------------------------------------
    img = cv::imread("../../03 Data/Simple Dataset Copied/" + image_str + ".png");
    std::vector<cv::Rect> rects;
    for (int i = 0; i < annotations.Rect_list.size(); i++)
    {
        rects.push_back(cv::boundingRect(annotations.Rect_list[i]));
    }

    util::print_bounding_boxes(img, rects, cv::Scalar(255, 255, 0));
    //for (int i = 0; i < end_points.size(); i++)
    //{
    //    cv::arrowedLine(img, rot_rects[i].center - end_points[i], rot_rects[i].center + end_points[i], cv::Scalar(200, 200, 0), 1, CV_AA);
    //}
    img_cropped = img(roi);
    cv::imshow("Upright bbox", img_cropped);
    imwrite(image_str+"_axis_aligned.png", img_cropped);

    cv::waitKey(0);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
