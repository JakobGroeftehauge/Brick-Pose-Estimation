#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

cv::Mat Hough_transform(cv::Mat img, float res_theta = CV_PI/180, float res_rho = 1)
{
    // Mat img - edge detected image
    // float res_theta - the resolution of the theta variable in radians. (default 1 degree)
    // float res_rho - the resolution of the rho variable in pixels.

    int img_width = img.size().width;
    int img_height = img.size().height;

    int rho_max = sqrt(pow(img_width, 2.0) + pow(img_height, 2.0)) + 0.5;
    int num_angle = (CV_PI * 2)/res_theta + 0.5; //find parameter (range/resolution)

    cout << "rho max" << rho_max << endl;
    cout << "num_angles" << num_angle << endl;

    //Create Accumulator array
    //cv::Mat accumulator_array = Mat::zeros(img.size().width, img.size().height, CV_8U);
    cv::Mat accumulator_array = Mat::zeros(num_angle, rho_max, CV_8U);


    for(int w = 0; w < img_width; w++)
    {
        for(int h = 0; h < img_height; h++)
        {
            //Check if the current pixel is a edge pixel (white)
            if(img.at<uchar>(cv::Point(w, h)) == 255)
            {
                cout << "found white pixel" << endl;
                for(float n = 0; n < num_angle; n++)
                {
                    float theta = n * res_theta;
                    int rho = cvRound(w * sin(theta) + h * cos(theta));
                    int rho_index =cvRound(rho_max/2 +  rho/res_rho);
                    accumulator_array.at<uchar>(cv::Point(rho_index, n))++;

                    //accumulator_array.at<uchar>(cv::Point(w, h))++; //For test
                }
            }
        }
    }
    return accumulator_array;
}


void save_to_csv(cv::Mat img)
{
    std::ofstream file;
    file.open("img.csv");

    for(int w = 0; w < img.size().width; w++)
    {
        for(int h = 0; h < img.size().height; h++)
        {
            file << (int) img.at<uchar>(cv::Point(w, h)) << ", ";
        }

        file << endl;
    }
    file.close();
}

int main()
{
    cv::Mat hough_space;
    cv::Mat edge_img;
    cv::Mat filter_img;
    cv::Mat gray_img = cv::imread("../Strojer_Images/Initial Test Images/Cropped/IMG_4042.JPG", IMREAD_GRAYSCALE);
    cv::medianBlur(gray_img, filter_img, 5);
    cv::Canny(filter_img, edge_img, 130, 200);



    hough_space = Hough_transform(edge_img);
    //save_to_csv(hough_space);

    cv::imshow("Grayscale Image", gray_img);
    cv::imshow("Filtered Image", filter_img);
    cv::imshow("Edge Image", edge_img);
    cv::imshow("Hough Space", hough_space);
    cout << "Hello World!" << endl;

    cv::waitKey(0);
    return 1;
}
