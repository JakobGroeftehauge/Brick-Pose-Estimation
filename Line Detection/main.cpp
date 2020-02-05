#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

// Image containers
cv::Mat img_full;
cv::Mat img;
cv::Mat img_copy;
cv::Mat img_gray;
cv::Mat img_gaussian;
cv::Mat img_canny;
cv::Mat img_hough;

// Display Windows
string win_img = "Raw Image";
string win_gaussian = "Gaussian Image";
string win_canny = "Canny Image";
string win_hough = "Hough Image";
string win_img_line = "Image with Lines";


//Global variables used for tracksbars.
// * Cannye Edge Detector
int canny_thresh1 = 61;
int canny_thresh2 = 125;
int canny_threshMax = 255;

//* Gaussian filter
int kernel_size[5] = {3, 5, 7, 9, 11};
int kernel_size_idx = 0;
int std_dev = 1;


//* Hough Lines
int hough_threshold = 172;
int rho = 1;
int theta = 1;






void equalize_luminance(cv::Mat& inputImg, cv::Mat& outputimg)
{
    cv::Mat hls_img;
    cv::cvtColor(inputImg, hls_img, cv::COLOR_BGR2HLS);

    std::vector<cv::Mat> channels(hls_img.channels());
    cv::split(hls_img, channels);
    cv::equalizeHist(channels[1], channels[1]);
    cv::merge(channels, hls_img);
    cv::cvtColor(hls_img, outputimg, cv::COLOR_HLS2BGR);
}


void change_images()
{
    cv::GaussianBlur(img_gray, img_gaussian, cv::Size(kernel_size[kernel_size_idx],kernel_size[kernel_size_idx]), std_dev,std_dev);
    cv::Canny(img_gaussian, img_canny, canny_thresh1, canny_thresh2);

    img_copy = img.clone();

    vector <Vec2f> lines;
    HoughLines(img_canny,lines, 1,CV_PI/180, hough_threshold);
    for (size_t i = 0; i < lines.size();i++)
    {
        float rho =  lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        int size = min(img_full.size().width, img_full.size().height);
        pt1.x=cvRound(x0 + size*(-b));
        pt1.y=cvRound(y0 + size*(a));
        pt2.x=cvRound(x0 - size*(-b));
        pt2.y=cvRound(y0 - size*(a));
        line(img_copy, pt1, pt2, Scalar(0,255,0), 1, LINE_AA);
    }
    cout << "Length of lines " << lines.size()<< endl;
}

void update_images()
{
     imshow(win_img, img);
     imshow(win_gaussian, img_gaussian);
     imshow(win_canny, img_canny);
     imshow(win_img_line,img_copy);
}

static void on_trackbar(int, void*)
{
    change_images();
    update_images();
}


int main()
{
    //load image

    img_full = cv::imread("../Strojer_Images/High-Res/snap1080_3_Color.png", IMREAD_COLOR);
    //img_full = cv::imread("../Strojer_Images/Lower-Res (from c++)/Color_RGB/59rs-save-to-disk-output-Color.png", IMREAD_COLOR);
   // equalize_luminance(img_full, img_full);

    //Crop image
    cv::Rect roi;
    //roi.x = 100;
    roi.x = 400;
    roi.y = 0;
    roi.width = min(img_full.size().width, img_full.size().height);
    roi.height = min(img_full.size().width, img_full.size().height);
    img = img_full(roi);

    // Create Display windows
    cv::namedWindow(win_img);
    cv::namedWindow(win_gaussian);
    cv::namedWindow(win_canny);
    cv::namedWindow(win_img_line);

    //Create Trackbars
    char TrackbarName[50];
    sprintf( TrackbarName, "Canny Threshold 1", canny_threshMax );
    cv::createTrackbar(TrackbarName, win_canny, &canny_thresh1, canny_threshMax, on_trackbar);
    sprintf( TrackbarName, "Canny Threshold 2", canny_threshMax );
    cv::createTrackbar(TrackbarName, win_canny, &canny_thresh2, canny_threshMax, on_trackbar);
    sprintf( TrackbarName, "Gaussian kernel");
    cv::createTrackbar(TrackbarName, win_gaussian, &kernel_size_idx, 4, on_trackbar);
    sprintf( TrackbarName, "Gaussian Std. dev.");
    cv::createTrackbar(TrackbarName, win_gaussian, &std_dev, 6, on_trackbar);
    sprintf( TrackbarName, "Hough Threshold");
    cv::createTrackbar(TrackbarName, win_img_line, &hough_threshold, 400, on_trackbar);
    /*
    sprintf( TrackbarName, "Hough rho");
    cv::createTrackbar(TrackbarName, win_img_line, &rho, 3, on_trackbar);
    sprintf( TrackbarName, "Hough theta");
    cv::createTrackbar(TrackbarName, win_img, &theta, 20, on_trackbar);*/
    //convert image to grayscale
    cv::cvtColor(img, img_gray, CV_BGR2GRAY);
    change_images();
    update_images();
    waitKey(0);

    // print parameters

    //waitKey(0);
    return 0;
}
