#include "Hough_space.h"

Hough_space::Hough_space()
{
}

Hough_space::Hough_space(cv::Mat edge_img)
{
    this->rng(123456);
	this->edge_img = edge_img.clone();
	this->img_height = this->edge_img.size().height;
	this->img_width = this->edge_img.size().width;
	this->rho_max = cvRound(sqrt(pow(img_width, 2.0) + pow(img_height, 2.0))/2);
	this->resolution_theta = CV_PI / 180;
	this->resolution_rho = 1;
	this->num_angle = cvRound(CV_PI / resolution_theta);
    this->hough_height = num_angle;
    this->hough_width = rho_max * 2 + 1;
	this->hough_matrix = cv::Mat::zeros(hough_height, hough_width, CV_16U);
    this->accum_threshold = 0.30;
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
                    this->hough_matrix.at<ushort>(cv::Point(rho_index, n))++;
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

std::vector<std::vector<double>>  Hough_space::find_lines()
{
    //cv::imshow("hough space reg", this->hough_matrix);
    cv::Mat hough_space_norm;

    cv::normalize(this->hough_matrix, hough_space_norm, 0, 65535, cv::NORM_MINMAX, CV_16U);
    hough_space_norm.convertTo(hough_space_norm, CV_8U, 1.0/256.0);
    cv::Mat hough_space_bin;// = threshold(hough_space_norm, accum_threshold);
    cv::threshold(hough_space_norm, hough_space_bin, cvRound(255*this->accum_threshold), 65535, cv::THRESH_BINARY);
    hough_space_bin.convertTo(hough_space_bin, CV_16U, 256.0);
 
    //cv::imshow("hough_space norm", hough_space_norm);
    int split_value = 0;
    if (clusters_at_border(hough_space_bin))
    {
        split_value = find_splitting_line(hough_space_bin);
        this->split_angle = split_value * this->resolution_theta;
        //shift the hough space
        if (split_value > 0)
        {
            shift_hough(hough_space_bin, hough_space_bin, split_value);
        }
        else
        {
            std::cout << "Could not find appropriate splitting line" << std::endl;
        }
    }
    //Dilate the clusters to "grow" them together
    //cv::imshow("hough space bin", hough_space_bin);
    cv::Mat kernel = cv::Mat::ones(3, 3, CV_8U);
    //cv::waitKey(0);
    for (int i = 0; i < 3; i++) // The number of dialations (i) needs to tuned.
    {
        dilate(hough_space_bin, hough_space_bin, kernel);
    }
    cv::Mat padded_hough_space;
    cv::copyMakeBorder(hough_space_bin, padded_hough_space, 1, 1, 1, 1, 0);//contours ignore border
    std::vector<std::vector<cv::Point> > contour_perimeters;
    std::vector<cv::Vec4i> hierarchy;
    padded_hough_space.convertTo(padded_hough_space, CV_8U);
    //cv::imshow("paddedhough", padded_hough_space);
    //cv::waitKey(0);
    // may  throw exception if no white points exist.
    findContours(padded_hough_space, contour_perimeters, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(-1, -1)); //try RETR_FLOODFILL 

    std::vector<std::vector<cv::Point>> complete_contours;
    for (unsigned int i = 0; i < contour_perimeters.size(); i++)
    {
        cv::Rect bounding_box = cv::boundingRect(contour_perimeters[i]);
        std::vector<cv::Point> contour;
        for (int j = 0; j < bounding_box.width; j++)
        {
            for (int u = 0; u < bounding_box.height; u++)
            {
                contour.push_back(cv::Point(bounding_box.x + j, bounding_box.y + u));
            }
        }
        complete_contours.push_back(contour);
        contour.clear();

    }
    
    // --------------------------------- Using Maximum -------------------------------------
    complete_contours = inverse_shift_points(complete_contours, split_value);

    show_contours(complete_contours);

    std::vector<std::vector<double>> lines;
    //std::cout << "Number of contours detected: " << complete_contours.size() << std::endl; //For debugging.

    //Find maximum point for every contour.
    for (unsigned int i = 0; i < complete_contours.size(); i++)
    {
        lines.push_back(convert_to_line(get_maximum(complete_contours[i])));
        //lines.push_back(convert_to_line(get_weighted_average(complete_contours[i])));
        //weighted average does not work due to the current structure. See explanation at definition.
    }
    // -------------------------------- using weighted average ------------------------------
    //show_contours(complete_contours);

    //std::vector<std::vector<double>> lines;
    //std::cout << "Number of contours detected: " << complete_contours.size() << std::endl; //For debugging.

    ////Find maximum point for every contour.
    //for (unsigned int i = 0; i < complete_contours.size(); i++)
    //{
    //    //lines.push_back(convert_to_line(get_maximum(complete_contours[i])));
    //    lines.push_back(convert_to_line(inverse_shift_single_point(get_weighted_average(complete_contours[i],split_value),split_value)));
    //    //weighted average does not work due to the current structure. See explanation at definition.
    //}
    return lines;
}

/**
 * Find whether there exists prominent clusters near the edges of the hough space, which would "wrap around"
 */
int Hough_space::clusters_at_border(cv::Mat hough_space_bin)
{
    int cluster_at_top = 0;
    int cluster_at_bot = 0;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < this->hough_width; col++)
            if (hough_space_bin.at<ushort>(cv::Point(col, row)) > this->accum_threshold)
            {
                cluster_at_top++;
                break;
            }
        if (cluster_at_top > 0)
            break;
    }
    for (int row = this->hough_height-1; row > hough_height - 1 - 3; row--)
    {
        for (int col = 0; col < this->hough_width; col++)
            if (hough_space_bin.at<ushort>(cv::Point(col, row)) > this->accum_threshold)
            {
                cluster_at_bot++;
                break;
            }
        if (cluster_at_bot > 0)
            break;
    }
    if (cluster_at_bot > 0 && cluster_at_top > 0)
        return 1;
    else
        return 0;
}

int Hough_space::find_splitting_line(cv::Mat hough_space_bin)
{
    // check whether 
    int result = 1;
    int counter = 0;
    for (int row = 0; row < this->hough_height; row++)
    {
        for (int col = 0; col < this->hough_width; col++)
        {
            if (hough_space_bin.at<ushort>(cv::Point(col, row)) > this->accum_threshold)
            {
                counter = 0;
                break;
            }

            if (col == this->hough_width - 1)
                counter++;
            if (counter > 4)
            {
                    return row - 2;
            }
        }
    }
    return 0;
}

std::vector<std::vector<cv::Point>> Hough_space::inverse_shift_points(std::vector<std::vector<cv::Point>> contours, int theta_split)
{
    std::vector<std::vector<cv::Point>> trans_point;
    for (unsigned int i = 0; i < contours.size(); i++)
    {
        std::vector<cv::Point> single_contour;
        for (unsigned int j = 0; j < contours[i].size(); j++)
        {
            cv::Point point = contours[i][j];
            if (point.y >= this->hough_height - theta_split) // >= since the split line is included in shifted region
            {
                int x_coor = this->hough_width - 1 - point.x;
                single_contour.push_back(cv::Point(x_coor, point.y - this->hough_height + theta_split));
            }
            else
            {
                single_contour.push_back(cv::Point(point.x, point.y + theta_split));
            }
        }
        trans_point.push_back(single_contour);
    }
    return trans_point;
}


std::vector<cv::Point2f> Hough_space::inverse_shift_single_contour(std::vector<cv::Point2f> contour, int theta_split)
{
    std::vector<cv::Point2f> single_contour;
    for (unsigned int i = 0; i < contour.size(); i++)
    {
        
        cv::Point point = contour[i];
        if (point.y >= this->hough_height - theta_split) // >= since the split line is included in shifted region
        {
            int x_coor = this->hough_width - 1 - point.x;
            single_contour.push_back(cv::Point(x_coor, point.y - this->hough_height + theta_split));
        }
        else
        {
            single_contour.push_back(cv::Point(point.x, point.y + theta_split));
        }
    }
    return single_contour;
}

cv::Point2f Hough_space::inverse_shift_single_point(cv::Point2f point, int theta_split)
{
    cv::Point2f single_contour;
    if (point.y >= this->hough_height - theta_split) // >= since the split line is included in shifted region
    {
        int x_coor = this->hough_width - 1 - point.x;
        single_contour = (cv::Point(x_coor, point.y - this->hough_height + theta_split));
    }
    else
    {
        single_contour = (cv::Point(point.x, point.y + theta_split));
    }
    return single_contour;
}

void Hough_space::show_contours(std::vector<std::vector<cv::Point>> contour_list)
{
    cv::Mat img = cv::Mat::zeros(cv::Size(this->hough_width, this->hough_height), CV_8UC3);

    for (unsigned int i = 0; i < contour_list.size(); i++)
    {
        cv::Scalar color = cv::Scalar(this->rng.uniform(0, 255), this->rng.uniform(0, 255), this->rng.uniform(0, 255));
        drawContours(img, contour_list, i, color, 1, cv::LINE_AA);
    }
    //cv::imshow("Detected Contours in Hough Space", img);
}
/**
* Convert a point in hough space to a vector of the line parameters for the corresponding point.
* @param pos_hough_space - point in hough space
* @return vector of line parameters - {rho, theta}.
*/
std::vector<double> Hough_space::convert_to_line(cv::Point pos_hough_space)
{
    double theta = pos_hough_space.y * this->resolution_theta;
    double rho = (pos_hough_space.x - this->rho_max) * this->resolution_rho;
    std::vector<double> line_parameters = { rho, theta };

    return line_parameters;
}
/*
* Overloaded version of above method to accound for cv::Point2f
*/
std::vector<double> Hough_space::convert_to_line(cv::Point2f pos_hough_space)
{
    double theta = pos_hough_space.y * this->resolution_theta;
    double rho = (pos_hough_space.x - this->rho_max) * this->resolution_rho;
    std::vector<double> line_parameters = { rho, theta };

    return line_parameters;
}

/**
*  Finds the most dominante point within a specified contour in the hough space
*/
cv::Point Hough_space::get_maximum(std::vector<cv::Point> contour)
{
    cv::Point max_point;
    int max_value = 0;
    for (unsigned int i = 0; i < contour.size(); i++)
    {
        //cout << "Contour: " << int(hough_space.at<uchar>(contour[i])) << endl;
        if (this->hough_matrix.at<ushort>(contour[i]) > max_value)
        {
            max_point = contour[i];
            max_value = this->hough_matrix.at<ushort>(contour[i]);
        }
    }
    return max_point;
}

/*
Finding the weighted average does not work due to the fact that the idicies relating to one cluster "jumps"
due to the shift and later inverse shift. This results in an average that does not make sense. A work around
for this may be to either keep the entire hough space in the shifted condition and simply change the axes, or
to keep the contour indicies in the shifted state until after the weighted average. This would require the hough 
space to be artifcially shifted while calculating the weighted average.
*/
cv::Point2f Hough_space::get_weighted_average(std::vector<cv::Point> contour, int split_value)
{
    cv::Mat hough_matrix_copy = this->hough_matrix.clone();
    shift_hough(hough_matrix_copy, hough_matrix_copy, split_value);
    cv::Point2f point_sum = {0,0};
    int accumulator_sum = 0;
    for (unsigned int i = 0; i < contour.size(); i++)
    {
        std::cout << "contour x: " << contour[i].x << " contour y: " << contour[i].y << " weight: " << hough_matrix_copy.at<ushort>(contour[i]) << std::endl;
        accumulator_sum += hough_matrix_copy.at<ushort>(contour[i]);
        point_sum.x += contour[i].x* hough_matrix_copy.at<ushort>(contour[i]);
        point_sum.y += contour[i].y * hough_matrix_copy.at<ushort>(contour[i]);
    }
    point_sum.x /= accumulator_sum;
    point_sum.y /= accumulator_sum;
    std::cout << "weighted x: " << point_sum.x << " weighted y: " << point_sum.y << std::endl;
    return point_sum;
}

void Hough_space::save_to_csv(std::string filename)
{
    {
        std::ofstream file;
        file.open(filename + ".csv");

        for (int w = 0; w < this->hough_matrix.size().width; w++)
        {
            for (int h = 0; h < this->hough_matrix.size().height; h++)
            {
                file << (int)this->hough_matrix.at<ushort>(cv::Point(w, h)) << ", ";
            }

            file << "\n";
        }

        file.close();
    }
}

void Hough_space::shift_hough(cv::Mat hough_space_src, cv::Mat& hough_space_dst, int split_value)
{
    cv::Mat hough_space_copy = hough_space_src.clone();

    cv::Mat transformed_hough;
    // Error below here when canny thresholds are low.
    // Maybe issue is if theta is = 0
    cv::Mat upper_part = hough_space_copy(cv::Rect(0, 0, this->hough_width, split_value)); // the split line is included in upper part
    cv::Mat lower_part = hough_space_copy(cv::Rect(0, split_value + 1, this->hough_width , this->hough_height - split_value -1));

    flip(upper_part, upper_part, 1);
    cv::vconcat(lower_part, upper_part, hough_space_dst);
}
