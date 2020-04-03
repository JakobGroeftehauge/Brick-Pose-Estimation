#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
//#include "predictions.h"
#include "../Includes/util.h"
#include "../Includes/annotation_loader.h"

int main()
{
    cv::Mat img = cv::imread("../../03 Data/Simple Dataset Copied/colorIMG_156.png");
    Annotation_loader annotations;
    annotations.loadAnnotation("../../03 Data/Simple Dataset Copied/colorIMG_156.json");
    cv::Mat overlay = cv::Mat::zeros(img.size(),img.type());
    cv::Mat fills = img.clone();
    std::cout << "Hello World!\n" << img.type();;
    int pt_radius = 3;
    cv::Scalar dot_color(100, 0, 160);
    cv::Scalar line_color(100, 0, 160);
    cv::Scalar fill_color(100, 0, 180);
    for (int i = 0; i < annotations.Rect_list.size(); i++)
    {
        std::vector<cv::Point2i> points;  //{ cv::Point(2,10), cv::Point(30,10), cv::Point(300,50), cv::Point(2,50) };
        cv::Mat(annotations.Rect_list[i]).convertTo(points, cv::Mat(points).type());
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

    cv::imshow("fills", fills);
    cv::imshow("overlay", overlay);
    cv::imshow("img", img);

    cv::Rect roi(cv::Point(210, 130), cv::Point(595, 480));
    cv::Mat img_cropped = img(roi);
    cv::imshow("img", img_cropped);
    imwrite("colorIMG_12_annotated.png", img_cropped);

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
