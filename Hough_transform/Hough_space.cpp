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
    fill_ticks();

}

/**
 * Method that initialises the hough space by calculating the hough transform and filling in "tick containers"
 * By default the reuolution of the hough space is 1 degreee for the theta, and 1 pixel for rho.
 *
 * @param img - edge image
 *
 * @return hough space
 */
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

/**
 * Method which fills in the "tick containers" for rho and theta.
 * These containers are implemented to ease conversion from image matrix to parameters.
 */
void Hough_space::fill_ticks()
{
    this->theta_tick.resize(this->num_angle);
    for (int i = 0; i < this->num_angle; i++)
    {
        this->theta_tick[i] = i * this->resolution_theta;
    }
    this->rho_tick.resize((this->rho_max*2+1)/this->resolution_rho);
    for (int i = 0; i < (this->rho_max * 2 + 1) / this->resolution_rho; i++)
    {
        this->rho_tick[i] = i * this->resolution_rho - this->rho_max;
    }
}

void Hough_space::find_lines(int accum_threshold)
{

}

void Hough_space::save_to_csv(std::string filename)
{
    {
        std::ofstream file;
        file.open(filename + ".csv");

        for (int w = 0; w < this->hough_space.size().width; w++)
        {
            for (int h = 0; h < this->hough_space.size().height; h++)
            {
                file << (int)this->hough_space.at<ushort>(cv::Point(w, h)) << ", ";
            }

            file << "\n";
        }

        file.close();
    }
}
