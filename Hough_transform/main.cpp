#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

RNG rng(12345); //random generated used for determine color of contours when displaying them.

/**
 * Method which return the hough space an of edge image.
 * By default the reuolution of the hough space is 1 degreee for the theta, and 1 pixel for rho.
 *
 * @param img - edge image
 *
 * @return hough space
 */
cv::Mat Hough_transform(cv::Mat img)
{
    int img_width = img.size().width;
    int img_height = img.size().height;

    int rho_max = cvRound(sqrt(pow(img_width, 2.0) + pow(img_height, 2.0)));
    int num_angle = 180;
    float res_theta = CV_PI/num_angle;

    //Create Accumulator array
    cv::Mat accumulator_array = Mat::zeros(num_angle, rho_max, CV_8U);

    for(int w = 0; w < img_width; w++)
    {
        for(int h = 0; h < img_height; h++)
        {
            //Check if the current pixel is a edge pixel (white)
            if(img.at<uchar>(cv::Point(w, h)) == 255)
            {
                for(float n = 0; n < num_angle; n++)
                {
                    float theta = n * res_theta;
                    int rho = cvRound(w * cos(theta) + h * sin(theta));
                    int rho_index =cvRound(rho_max/2 +  rho);
                    accumulator_array.at<uchar>(cv::Point(rho_index, n))++;
                }
            }
        }
    }

    return accumulator_array;
}

/**
 * @brief Function which save a Mat container to a csv file.
 *
 * @param img - The imag@brief e to save.
 * @param filename - name of file (without suffix).
 */
void save_to_csv(cv::Mat img, std::string filename)
{
    std::ofstream file;
    file.open(filename + ".csv");

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

// Convert to taking a list of point insted.
void draw_lines(cv::Mat img, cv::Mat hough_space)
{
    int height = img.size().height;
    int width = img.size().width;
    float theta_res = (CV_PI)/hough_space.size().height;
    int rho_max = cvRound(sqrt(pow(height, 2) + pow(width, 2)));
    int rho_res = rho_max/hough_space.size().width;


    for(int h = 0; h < hough_space.size().height; h++)
    {
        for(int w = 0; w < hough_space.size().width; w++)
        {
            if(hough_space.at<uchar>(cv::Point(w, h)) == 255)
            {
                cv::Point pt1, pt2;
                double rho = (w - rho_max/2.0) * rho_res, theta = h * theta_res;
                double a = cos(theta), b = sin(theta);
                double x0 = a * rho, y0 = b * rho;
                int size = 1000;// max(height, width)*2;
                pt1.x=cvRound(x0 + size*(-b));
                pt1.y=cvRound(y0 + size*(a));
                pt2.x=cvRound(x0 - size*(-b));
                pt2.y=cvRound(y0 - size*(a));
                line(img, pt1, pt2, Scalar(0,255,0), 1, LINE_AA);
                cout << "rho: " << rho << "  theta: " << theta << endl;

            }
        }
    }
}

/**
 * Performs thresholding on the hough space.
 *
 * @param hough_space
 * @param threshold
 *
 * @return A binary map of the hough space. A pixel is colored white if
 * its value is equal of above thte threshold.
 */
cv::Mat threshold(cv::Mat hough_space, int threshold)
{
    int width = hough_space.size().width;
    int height = hough_space.size().height;
    int count = 0; //Only used for testing.

    cv::Mat thres_hough = Mat::zeros(height, width, CV_8U);
    for(int w = 0; w < width; w++)
    {
        for(int h = 0; h < height; h++)
        {
            if(hough_space.at<uchar>(cv::Point(w, h)) >= threshold)
            {
                thres_hough.at<uchar>(cv::Point(w, h)) = 255;
                count++;
            }
        }
    }
    cout << "NUMBER OF PIXELS IN THRESH IMAGE:" << count << endl;
    return thres_hough;
}

/**
 *  Finds the most dominante point within a specified contour in the hough space
 */
cv::Point get_maximum(cv::Mat hough_space, vector<cv::Point> contour)
{
    cv::Point max_point;
    int max_value = 0;
    for(int i = 0; i < contour.size(); i++)
    {
        if(hough_space.at<uchar>(contour[i]) > max_value)
        {
            max_point = contour[i];
            max_value = hough_space.at<uchar>(contour[i]);
        }
    }
    return max_point;
}

/**
 * Function which finds the dominante lines in hough space
 *
 * @param hough_space
 *
 * @return vector of dominant lines.
 */
void get_dominant_lines(cv::Mat hough_space)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

   cv::Mat kernel = Mat::ones(3, 3, CV_8U);
    for(int i = 0; i < 3; i++)
    {
        dilate(hough_space, hough_space, kernel);
    }

    cv::Mat padded_hough_spac;
    cv::copyMakeBorder(hough_space, padded_hough_spac, 1, 1, 1, 1, 0);

    findContours(padded_hough_spac, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cv::Point(-1,-1));
    Mat drawing = Mat::zeros( hough_space.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
       {
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
       }
    cv::imshow("contours", drawing);
    int contour_num = 3;
    for(int i = 0; i < contours[contour_num].size(); i++)
    {
        cout << "Point " << i << ": " << contours[contour_num][i] << endl;
    }
    cv::waitKey(0);
}



int main()
{
    cv::Mat hough_space;
    cv::Mat edge_img;
    cv::Mat filter_img;
    cv::Mat gray_img;
    cv::Mat hough_space_norm;
    cv::Mat color_img = cv::imread("../Strojer_Images/Initial Test Images/Cropped/IMG_4050.JPG", IMREAD_COLOR);
    cv::cvtColor(color_img, gray_img, CV_BGR2GRAY);
    cv::medianBlur(gray_img, filter_img, 5);
    cv::Canny(filter_img, edge_img, 130, 200);

    hough_space = Hough_transform(edge_img);
    save_to_csv(hough_space, "test");
    cv::normalize(hough_space, hough_space_norm, 0, 255, NORM_MINMAX, CV_8UC1);

    // Find the maximum value in the hough_space
    double maxVal;
    cv::minMaxLoc(hough_space, NULL, &maxVal);
    cv::imshow("Hough Nomralized", hough_space_norm);
    cout << "Max value: " << maxVal << endl;
    hough_space = threshold(hough_space, 70);
    //save_to_csv(hough_space);
    draw_lines(color_img, hough_space);
    get_dominant_lines(hough_space);
    cv::imshow("Color_image", color_img);
    cv::imshow("Grayscale Image", gray_img);
    cv::imshow("Filtered Image", filter_img);
    cv::imshow("Edge Image", edge_img);
    cv::imshow("Hough Space thres", hough_space);
    cout << "Hello World!" << endl;


    cv::waitKey(0);
    return 1;
}
