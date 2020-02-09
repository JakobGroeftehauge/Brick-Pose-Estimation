#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

RNG rng(12345); //random generator used for determine color of contours when displaying them.
double resolution_theta = CV_PI/180;
int resolution_rho = 1;

/**
 * Convert a point in hough space to a vector of the line parameters for the corresponding point.
 *
 * @param pos_hough_space - point in hough space
 * @param rho_max - size of the hough space in the rho dimension
 *
 * @return vector of line parameters - {rho, theta}.
 */
std::vector<double> convert_to_line(cv::Point pos_hough_space, int rho_range)
{
    double theta = pos_hough_space.y * resolution_theta;
    double rho = (pos_hough_space.x - rho_range/2.0) * resolution_rho;
    vector<double> line_parameters = {rho, theta};

    return line_parameters;
}

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

    //Create Accumulator array
    int rho_max = cvRound(sqrt(pow(img_width, 2.0) + pow(img_height, 2.0)));
    int num_angle = CV_PI/resolution_theta;
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
                    float theta = n * resolution_theta;
                    int rho = cvRound(w * cos(theta) + h * sin(theta));
                    int rho_index =cvRound(rho_max/2 + rho);
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
    pt1.x=cvRound(x0 + size*(-b));
    pt1.y=cvRound(y0 + size*(a));
    pt2.x=cvRound(x0 - size*(-b));
    pt2.y=cvRound(y0 - size*(a));
    line(img, pt1, pt2, Scalar(0,255,0), 1, LINE_AA);
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

    cv::Mat thres_hough = Mat::zeros(height, width, CV_8U);
    for(int w = 0; w < width; w++)
    {
        for(int h = 0; h < height; h++)
        {
            if(hough_space.at<uchar>(cv::Point(w, h)) >= threshold)
            {
                thres_hough.at<uchar>(cv::Point(w, h)) = 255;
            }
        }
    }
    return thres_hough;
}

/**
 *  Finds the most dominante point within a specified contour in the hough space
 */
cv::Point get_maximum(cv::Mat hough_space, vector<cv::Point> contour)
{
    cv::Point max_point;
    int max_value = 0;
    for(unsigned int i = 0; i < contour.size(); i++)
    {
        if(hough_space.at<uchar>(contour[i]) > max_value)
        {
            max_point = contour[i];
            max_value = hough_space.at<uchar>(contour[i]);
        }
    }
    return max_point;
}


vector<vector<cv::Point>> find_contours(cv::Mat hough_space_bin)
{
    cv::Mat hough_space_copy = hough_space_bin.clone();

    cv::Mat kernel = Mat::ones(3, 3, CV_8U);
    for(int i = 0; i < 2; i++) // The number of dialations (i) needs to tuned.
    {
        dilate(hough_space_copy, hough_space_copy, kernel);
    }

    // Pad hough space with 1 pixels on all edges, due to the function
    // find contours ignores the outer most pixel.
    cv::Mat padded_hough_space;
    cv::copyMakeBorder(hough_space_copy, padded_hough_space, 1, 1, 1, 1, 0);

    vector<vector<Point> > contour_perimeters;
    vector<Vec4i> hierarchy;

    findContours(padded_hough_space, contour_perimeters, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cv::Point(-1,-1));

    vector<vector<cv::Point>> complete_contours;
    for(unsigned int i = 0; i < contour_perimeters.size(); i++)
    {
        cv::Rect bounding_box = cv::boundingRect(contour_perimeters[i]);
        vector<cv::Point> contour;
        for(int j = 0; j < bounding_box.width; j++)
        {
            for(int u = 0; u < bounding_box.height; u++)
            {
                contour.push_back(cv::Point(bounding_box.x + j, bounding_box.y + u));
            }
        }
        complete_contours.push_back(contour);
        contour.empty();

    }

    //Combine contour which parameters yeilds similar line, but is not necesaary located next to eachother in hough space. (fx 0 and pi)
    // NEEDS TO BE FIXED.
    return complete_contours;
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

/**
 * Function which finds the dominante lines in hough space
 *
 * @param hough_space
 *
 * @return vector of dominant lines.
 */
vector<vector<double>> get_dominant_lines(cv::Mat hough_space, cv::Mat hough_space_bin)
{
    vector<vector<cv::Point>> contours = find_contours(hough_space_bin);
    show_contours(contours, hough_space.size().height, hough_space.size().width);

    vector<vector<double>> lines;
    cout <<"Number of contours detected: " <<  contours.size() << endl; //For debugging.

    //Find maximum point for every contour.
    for(unsigned int i = 0; i < contours.size(); i++)
    {
        lines.push_back(convert_to_line(get_maximum(hough_space, contours[i]), hough_space.size().width));
    }
    return lines;
}

vector<vector<double>> findLines(cv::Mat edge_img, int accum_threshold)
{
    cv::Mat hough_space = Hough_transform(edge_img);
    cv::Mat hough_space_norm;
    cv::normalize(hough_space, hough_space_norm, 0, 255, NORM_MINMAX, CV_8UC1);
    cv::Mat hough_space_bin = threshold(hough_space_norm, accum_threshold);
    cv::imshow("hough space bin", hough_space_bin);
    cv::imshow("hough_space norm", hough_space_norm);
    return get_dominant_lines(hough_space, hough_space_bin);
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


int main()
{
    cv::Mat hough_space;
    cv::Mat edge_img;
    cv::Mat filter_img;
    cv::Mat gray_img;
    cv::Mat hough_space_norm;
    cv::Mat color_img = cv::imread("../Strojer_Images/Initial Test Images/Cropped/IMG_4048.JPG", IMREAD_COLOR);

    cv::cvtColor(color_img, gray_img, CV_BGR2GRAY);
    cv::medianBlur(gray_img, filter_img, 5);
    cv::Canny(filter_img, edge_img, 130, 200);

    hough_space = Hough_transform(edge_img);
    save_to_csv(hough_space, "test");

    vector<vector<double>> lines = findLines(edge_img, 90);
    cout << "Number of lines detected: " << lines.size() << endl;
    print_line_parameters(lines);

    // Show lines
    draw_lines(color_img, lines);
    //print_line(color_img, -100, 3.12414);
    cv::imshow("Color_image", color_img); //Lines are not perfect due to holes in contour

    //Show images
    //cv::imshow("Grayscale Image", gray_img);
    //cv::imshow("Filtered Image", filter_img);
    cv::imshow("Edge Image", edge_img);
    cv::imshow("Hough Space", hough_space);

    cv::waitKey(0);
    return 1;
}
