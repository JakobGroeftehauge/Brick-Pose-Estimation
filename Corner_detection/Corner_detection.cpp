#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat src, src_gray, src_blurred;
int thresh = 200;
int max_thresh = 255;
const char* source_window = "Source image";
const char* blurred_window = "Blurred Image";
const char* corners_window = "Corners detected";
void cornerHarris_demo(int, void*);
int main(int argc, char** argv)
{
    //CommandLineParser parser(argc, argv, "{@input | building.jpg | input image}");
    src = cv::imread("../Strojer_Images/Initial Test Images/Cropped/IMG_4048.jpg", IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }

    cv::medianBlur(src,src_blurred,3);
    //cv::bilateralFilter(src,src_blurred,10,30,30);
    cvtColor(src_blurred, src_gray, COLOR_BGR2GRAY);
    namedWindow(source_window);
    createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
    imshow(source_window, src);
    namedWindow(blurred_window);
    imshow(blurred_window, src_blurred);

    cornerHarris_demo(0, 0);
    waitKey();
    return 0;
}
void cornerHarris_demo(int, void*)
{
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.01;
    Mat dst = Mat::zeros(src.size(), CV_32FC1);
    cornerHarris(src_gray, dst, blockSize, apertureSize, k);
    Mat dst_norm, dst_norm_scaled;
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);
    for (int i = 0; i < dst_norm.rows; i++)
    {
        for (int j = 0; j < dst_norm.cols; j++)
        {
            if ((int)dst_norm.at<float>(i, j) > thresh)
            {
                circle(dst_norm_scaled, Point(j, i), 5, Scalar(0), 2, 8, 0);
            }
        }
    }
    namedWindow(corners_window);
    imshow(corners_window, dst_norm_scaled);
}
