#include "Hough_space.h"

Hough_space::Hough_space(cv::Mat edge_img)
{
	this->edge_img = edge_img.clone();
	this->img_height = this->edge_img.size().height;
	this->img_width = this->edge_img.size().width;
	this->rho_max = cvRound(sqrt(pow(img_width, 2.0) + pow(img_height, 2.0))/2);
	this->resolution_theta = CV_PI / 180;
	this->resolution_rho = 1;
	this->num_angle = cvRound(CV_PI / resolution_theta);
	this->hough_space = cv::Mat::zeros(num_angle, rho_max*2+1, CV_16U);
    initialise_hough_space();

}

void Hough_space::initialise_hough_space()
{
    for (int w = 0; w < img_width; w++)
    {
        for (int h = 0; h < img_height; h++)
        {
            float theta;
            int rho;
            int rho_index;
            //Check if the current pixel is a edge pixel (white)
            if (edge_img.at<uchar>(cv::Point(w, h)) == 255)
            {
                for (float n = 0; n < num_angle; n++)
                {
                    theta = n * resolution_theta;
                    rho = cvRound((w - img_width / 2) * cos(theta) + (h - img_height / 2) * sin(theta));
                    rho_index = cvRound(rho_max + rho);
                    this->hough_space.at<ushort>(cv::Point(rho_index, n))++;
                }
            }
        }
    }
}
