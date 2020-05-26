#include <iostream>
#include <opencv2/opencv.hpp>
#include "../Includes/Chamfer_brick_detector.h"
#include "../Includes/util.h"


using namespace std;

std::string image_str = "OP_colorIMG_427";

int main()
{

    cv::Mat img = cv::imread("../../03 Data/Dataset2_onPallet/" + image_str + ".png");
    cv::Mat img_gray;
    cv::Mat img_edge;

    Chamfer_brick_detector detector;
    detector.set_brick_specs(139, 30, 211, 54.39);
    detector.set_resolution(60, 5);
    detector.set_NMS_thresh(175);
    detector.detect(img);

    cv::cvtColor(img, img_gray, CV_BGR2GRAY);

    cv::GaussianBlur(img_gray, img_gray, cv::Size(5, 5), 0, 0);

    cv::Canny(img_gray, img_edge, 25, 70);
    
    std::vector<bounding_box> true_pos;
    std::vector<bounding_box> false_pos;
    for (int i = 0; i < detector.predictions.size(); i++)
    {
        if (i == detector.predictions.size() - 3)
            false_pos.push_back(detector.predictions[i]);
        else
            true_pos.push_back(detector.predictions[i]);
    }

    util::print_rotated_bounding_boxes(img, true_pos, cv::Scalar(0,255,0), 2);
    util::print_rotated_bounding_boxes(img, false_pos, cv::Scalar(0,0,255),3);

    cv::Rect roi(540, 125, 600, 450);

    img = img(roi);
    img_edge = img_edge(roi);

    cv::imshow("img", img);
    cv::imshow("edge img", img_edge);
    cv::waitKey(0);

    cv::imwrite("OP_sample_raw.png", img);
    cv::imwrite("OP_sample_edge.png", img_edge);
    cout << "Hello World!" << endl;
    return 0;
}
