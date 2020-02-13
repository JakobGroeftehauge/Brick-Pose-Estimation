#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "Hough_space.h"
#include "Bricks_from_lines_img.h"

using namespace std;
using namespace cv;

RNG rng(12347); //random generator used for determine color of contours when displaying them.
double resolution_theta = CV_PI/180;
int resolution_rho = 1;

void save_to_csv(cv::Mat img, std::string filename)
{
    {
        std::ofstream file;
        file.open(filename + ".csv");

        for (int w = 0; w < img.size().width; w++)
        {
            for (int h = 0; h < img.size().height; h++)
            {
                file << (int)img.at<ushort>(cv::Point(w, h)) << ", ";
            }

            file << "\n";
        }

        file.close();
    }
}

/**
 * print a single line on an image.
 * @param img (image with 3 channels)
 * @param rho
 * @param theta
 */
void print_line(cv::Mat img, double rho, double theta)
{
    cv::Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a * rho, y0 = b * rho;
    int size = 1000;//max(img.size().height, img.size().width) * 2; //just needs to be big enough.
    pt1.x=cvRound(x0 + size*(-b)) + img.size().width/2;
    pt1.y=cvRound(y0 + size*(a)) + img.size().height/2;
    pt2.x=cvRound(x0 - size*(-b)) + img.size().width/2;
    pt2.y=cvRound(y0 - size*(a)) + img.size().height/2;
    line(img, pt1, pt2, Scalar(0,255,0), 1, LINE_AA);
}

/**
 * print a single line on a black and white image.
 * @param img (image with 1 channel)
 * @param rho
 * @param theta
 */
void print_line_bw(cv::Mat img, double rho, double theta)
{
    cv::Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a * rho, y0 = b * rho;
    int size = 1000;//max(img.size().height, img.size().width) * 2; //just needs to be big enough.
    pt1.x = cvRound(x0 + size * (-b)) + img.size().width / 2;
    pt1.y = cvRound(y0 + size * (a)) + img.size().height / 2;
    pt2.x = cvRound(x0 - size * (-b)) + img.size().width / 2;
    pt2.y = cvRound(y0 - size * (a)) + img.size().height / 2;
    line(img, pt1, pt2, 0, 2);
}

/**
 * Draw lines on an image
 * @param img
 *
 */
void draw_lines(cv::Mat img, vector<vector<double>> lines)
{
    for(unsigned int i = 0; i < lines.size(); i++)
    {
        print_line(img, lines[i][0], lines[i][1]);
    }
}

/**
 * @brief Cand show the found regions of ineterest within the hough space.
 * @param contour_list - list of contours detected in the hough space.
 * @param height - size of hough space in the theta dimension.
 * @param width - size of hough spaec in the rho dimension.
 */
void show_contours(vector<vector<cv::Point>> contour_list, int height, int width)
{
    cv::Mat img = Mat::zeros(cv::Size(width, height), CV_8UC3);

    for(unsigned int i = 0; i < contour_list.size(); i++)
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( img, contour_list, i, color, 1, LINE_AA);
    }
    cv::imshow("Detected Contours in Hough Space", img);
}

void print_line_parameters(vector<vector<double>> lines)
{
    cout << "Parameters of found lines " << endl;
    for(unsigned int i = 0; i < lines.size(); i++)
    {
        cout << "Line " << (i + 1) << " -  Theta: " << lines[i][1] << "   Rho: " << lines[i][0] << endl;
    }
    cout << "\n";
}

// Finds the intersection of two lines, or returns false.
// The lines are defined by (o1, p1) and (o2, p2).
/**
 * Find the intersection between two lines or return false. If true the found
 * @param o1
 * @param p1
 * @param o2
 * @param p2
 * @param r
 * @return
 */
bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r)
{
    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < /*EPS*/1e-8)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    r = o1 + d1 * t1;
    return true;
}

/**
 * Given a set of line parameters. The function return two points located on line far apprt. (Used for calculaing the intersection between two lines)
 * @param line_parameters
 * @param img_width
 * @param img_height
 */
vector<Point2f> get_points_on_line(vector<double> line_parameters, int img_width, int img_height)
{
    cv::Point2f pt1, pt2;
    int rho = line_parameters[0];
    double theta = line_parameters[1];
    double a = cos(theta), b = sin(theta);
    double x0 = a * rho, y0 = b * rho;
    int size = 1000; //Increase if image size increases.
    pt1.x = x0 + size*(-b) + img_width/2;
    pt1.y = y0 + size*(a) + img_height/2;
    pt2.x = x0 - size*(-b) + img_width/2;
    pt2.y = y0 - size*(a) + img_height/2;
    return {pt1, pt2};
}

/**
 * Reduce the detected intersections to only include the intersections located with in the image.
 * @param intersections - vector of intersection points.
 * @param max_x - image width
 * @param max_y - image height
 */
vector<Point2f> filter_intersections(vector<cv::Point2f> intersections, int max_x, int max_y)
{
    vector<cv::Point2f> filtered_intersections;
    for(unsigned int i = 0; i < intersections.size(); i++)
    {
        cv::Point2f point = intersections[i];
        if(point.x < (float) max_x && point.x > 0.0 && point.y < (float) max_y && point.y > 0.0)
        {
            filtered_intersections.push_back(point);
            //cout << "filt x: " << point.x << " - " <<point.y << endl;
        }
    }
    return filtered_intersections;
}

/**
 * Finds intersections between lines.
 * @param lines - vector of line paramaters {rho, theta}
 * @param img_width, img height.
 * @return vector of intersections (cv::Point2f). The coordinates of the intersections relates directly to the image coordinates.
 */
vector<cv::Point2f> get_intersections(vector<vector<double>> lines, int img_width, int img_heigt)
{
    vector<cv::Point2f> intersections;
    cv::Point2f intersection_point;
    vector<cv::Point2f> points_line1;
    vector<cv::Point2f> points_line2;

    for(unsigned int i = 0; i < lines.size() - 1; i++)
    {
        points_line1 = get_points_on_line(lines[i], img_width, img_heigt);
        for(unsigned int j = i + 1; j < lines.size() ; j++)
        {
            points_line2 = get_points_on_line(lines[j], img_width, img_heigt);
            if(intersection(points_line1[0], points_line1[1], points_line2[0], points_line2[1], intersection_point) == true)
            {
                intersections.push_back(intersection_point);
            }
        }
    }

    return filter_intersections(intersections, img_width, img_heigt);
}

/**
 * prints the intersections on an image.
 * @param img - color image
 * @param points - vector of points
 */
void show_intersections(cv::Mat img, vector<cv::Point2f> points)
{
    for(unsigned int i = 0; i < points.size(); i++)
    {
        cv::circle(img, points[i], 2, cv::Scalar(0,0,255), -1);
    }
}

/**
 * prints the detected intersections in the terminal.
 * @param intersections - vector of points
 */
void print_intersections(vector<cv::Point2f> intersections)
{
    cout << "Detected intersections: " << endl;

    for(unsigned int i = 0; i < intersections.size(); i++)
    {
        cout << "Point " << i << "-  x:" << intersections[i].x << "  y: "<< intersections[i].y << endl;
    }
}

int main()
{
    cv::Mat hough_space;
    cv::Mat edge_img;
    cv::Mat filter_img;
    cv::Mat gray_img;
    cv::Mat hough_space_norm;
    cv::Mat color_img = cv::imread("../Strojer_Images/Initial Test Images/Cropped/IMG_4047.JPG", IMREAD_COLOR);

    cv::cvtColor(color_img, gray_img, CV_BGR2GRAY);
    cv::medianBlur(gray_img, filter_img, 5);
    cv::Canny(filter_img, edge_img, 60, 140);
    cv::imshow("Edge Image", edge_img);

    Hough_space hough_lines(edge_img);

    hough_lines.save_to_csv("test");

    vector<vector<double>> lines = hough_lines.find_lines();
    cout << "Number of lines detected: " << lines.size() << endl;
    print_line_parameters(lines);

    Bricks_from_lines_img brick_detections(color_img,lines);
    brick_detections.draw_rect(color_img);
    brick_detections.draw_rotated_rect(color_img);

    //draw_lines(color_img, lines);
    //print_line(color_img, 114, 3.14);

    vector<cv::Point2f> intersections = get_intersections(lines, color_img.size().width, color_img.size().height);
    show_intersections(color_img, intersections);
    print_intersections(intersections);

    cv::imshow("Color_image", color_img); //Lines are not perfect due to holes in contour

    cv::waitKey(0);
    return 1;
}
