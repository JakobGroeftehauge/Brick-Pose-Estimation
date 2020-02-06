#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

cv::Mat Hough_transform(cv::Mat img)
{
    // Mat img - edge detected image

    //Create Accumulator array
    cv::Mat accumulator_array = Mat::zeros(img.size().width, img.size().height, CV_8U);

    int num_angle = 255; //find parameter (range/resolution)

    for(int w = 0; w < img.size().width; w++)
    {
        for(int h = 0; h < img.size().height; h++)
        {
            //Check if the current pixel is a edge pixel (white)
            if(img.at<uchar>(cv::Point(w, h)) == 255)
            {
                cout << "found white pixel" << endl;
                for(float n = 0; n < num_angle; n++)
                {
                    float theta = 0;
                    int rho = cvRound(w*sin(theta) + h*cos(theta));
                    //int rho_index =
                    //accumulator_array.at<uchar>(cv::Point(theta, rho_index))++;
                    accumulator_array.at<uchar>(cv::Point(w, h))++;
                }
            }
        }
    }
    return accumulator_array;
}


int main()
{
    cv::Mat hough_space;
    cv::Mat edge_img;
    cv::Mat filter_img;
    cv::Mat gray_img = cv::imread("../Strojer_Images/Initial Test Images/Cropped/IMG_4047.JPG", IMREAD_GRAYSCALE);
    cv::medianBlur(gray_img, filter_img, 5);
    cv::Canny(filter_img, edge_img, 22, 110);

    cv::imshow("Grayscale Image", gray_img);
    cv::imshow("Filtered Image", filter_img);
    cv::imshow("Edge Image", edge_img);

    hough_space = Hough_transform(edge_img);
    cv::imshow("Hough Space", hough_space);

    cout << "Hello World!" << endl;
    cv::waitKey(0);
    return 0;
}
