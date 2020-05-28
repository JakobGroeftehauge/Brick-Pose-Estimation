#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "../Includes/predictions.h"
#include "../Includes/util.h"
#include "../Includes/annotation_loader.h"

void print_angle_predictions(cv::Mat& img, bounding_box BB, int line_width, cv::Scalar color = cv::Scalar(255, 255, 0))
{
    double w = BB.rotated_rect.size.width;
    double h = BB.rotated_rect.size.height;
    double angle = BB.rotated_rect.angle;
    cv::Point2d pt1 = BB.rotated_rect.center + cv::Point2f(w / 2.0 * sin((angle + 90) * CV_PI / 180) * 0.5, -w / 2.0 * cos((angle + 90) * CV_PI / 180) * 0.5);
    cv::Point2d pt2 = BB.rotated_rect.center - cv::Point2f(w / 2.0 * sin((angle + 90) * CV_PI / 180) * 0.5, -w / 2.0 * cos((angle + 90) * CV_PI / 180) * 0.5);

    cv::arrowedLine(img, pt2, pt1, color, line_width, CV_AA, 0, 0.15);
}

void draw_rot_rect(cv::Mat& img, cv::RotatedRect rot_rect, cv::Scalar color, int line_width)
{
    cv::Point2f rect_points[4];
    rot_rect.points(rect_points);

    for (int j = 0; j < 4; j++)
    {
        cv::line(img, rect_points[j], rect_points[(j + 1) % 4], color, line_width, CV_AA);
    }
}

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
    
    //------------------- Load annotations into bounding box struct -----------------------------
    std::vector<bounding_box> bounding_boxes;
    for (int i = 0; i < annotations.Rect_list.size(); i++)
    {
        bounding_boxes.push_back(annotations.Rect_list[i]);
    }


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
        //cv::arrowedLine(img, rot_rects[i].center-end_points[i], rot_rects[i].center+end_points[i], cv::Scalar(255, 255, 0),1,CV_AA);
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
    std::vector<std::vector<cv::Point2f>> AABB_points;
    for (int i = 0; i < annotations.Rect_list.size(); i++)
    {
        rects.push_back(cv::boundingRect(annotations.Rect_list[i]));
    }

    util::print_bounding_boxes(img, rects, cv::Scalar(255, 255, 0));
    std::vector<cv::Point2f > points_;
    for (int i = 0; i < end_points.size(); i++)
    {
        points_.clear();
        points_.push_back(rects[i].br());
        points_.push_back(rects[i].tl());
        cv::arrowedLine(img, rot_rects[i].center - end_points[i], rot_rects[i].center + end_points[i], cv::Scalar(255, 255, 0), 2, CV_AA);
        util::draw_points(img, points_, pt_radius, dot_color);
    }
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
