#include <iostream>
#include "../Includes/Brick_Detector.h"
#include "../Includes/util.h"


using namespace std;

void img_1(cv::Mat &img, Brick_Detector &detector)
{
    util::print_lines(img, detector.lines, cv::Scalar(200, 0, 0));

    for(int i = 0; i < detector.intersection_mat.size(); i++)
    {
        std::vector<cv::Point2f> points = detector.intersection_mat[i];
        for(int j = 0; j < points.size(); j++)
        {
            circle(img, points[j], 3, cv::Scalar(0,0,255), cv::FILLED, CV_AA);
            string number = to_string(i * points.size() + j);
            putText(img, number, points[j]+ cv::Point2f(5,2), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.55, cv::Scalar(255, 255, 255), 1, 8, false);
        }
    }
}

void img_2(cv::Mat &img, Brick_Detector &detector)
{
    util::print_bounding_boxes(img, detector.predictions, cv::Scalar(0,255, 0));
}

int main()
{
    cv::Mat img = cv::imread("../../03 Data/Simple Dataset/colorIMG_12.png");
    cv::imshow("raw img", img);
    Brick_Detector detector(img);
    detector.detect();
    cv::Mat hough = detector.hough.hough_matrix;
    cv::Mat hough_rot = detector.hough.hough_rotated;

    double min, max;
    cv::minMaxLoc(hough, &min, &max);
    std::cout << min << "  " << max << std::endl;
    cv::normalize(hough, hough, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    cv::imshow("hough space", hough);


    cv::imshow("hough space rot", hough_rot);

    cout << "Hello World!" << endl;
    cout << detector.hough.split_angle << endl;
    cv::Mat img_lines = img.clone();
    cv::Mat img_detections = img.clone();

    img_1(img_lines, detector);
    img_2(img_detections, detector);
    cv::imshow("img lines", img_lines);
    cv::imshow("predictions", img_detections);

    //For the preprocessing section
    cv::imshow("edge_img1", detector.hough.edge_img);

    //Save images to disk
    imwrite("edge_img.png", detector.hough.edge_img);
    imwrite( "hough_space.png", hough );
    imwrite( "hough_space_rot.png", hough_rot);
    imwrite( "lines_hough.png", img_lines);
    imwrite( "BB_hough.png", img_detections);

    cv::waitKey(0);
    return 0;
}
