#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;

std::string image_str = "OP_colorIMG_104";

int main()
{

    cv::Mat img = cv::imread("../../03 Data/Dataset2_onPallet/" + image_str + ".png");
    cv::Mat img_gray;
    cv::Mat img_edge;

    cv::cvtColor(img, img_gray, CV_BGR2GRAY);

    cv::medianBlur(img_gray, img_gray, 5);

    cv::Canny(img_gray, img_edge, 25, 70);

    cv::imshow("img", img);
    cv::imshow("edge img", img_edge);
    cv::waitKey(0);

    cv::imwrite("OP_sample_raw.png", img);
    cv::imwrite("OP_sample_edge.png", img_edge);
    cout << "Hello World!" << endl;
    return 0;
}
