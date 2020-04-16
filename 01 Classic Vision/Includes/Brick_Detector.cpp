#include "Brick_Detector.h"

Brick_Detector::Brick_Detector()
{
    set_canny_thresh(60, 140);
}

Brick_Detector::Brick_Detector(cv::Mat img)
{
    this->img = img;
    set_canny_thresh(60, 140);
}

void Brick_Detector::detect()
{
    // ---------------- for timing -------------------
    std::chrono::high_resolution_clock::time_point end;
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::duration<float> duration;
    start = std::chrono::high_resolution_clock::now();
    // ------------------------------------------------
    clear_predictions();
    find_lines();

    if(lines.size() > 0)
    {
        std::vector<std::vector<std::vector<double>>> clustered_lines = cluster_lines();

        if(clustered_lines[0].size() > 0 && clustered_lines[1].size() > 0)
        {
            find_BB(clustered_lines);
        }
        else
        {
            //TO DO: determine what to do when none or only one cluster is located.
        }
    }
    else
    {
       return;  //TO DO: determine what to do when no lines has been located.
    }
    // ---------------- for timing -------------------
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    float ms = duration.count() * 1000.0f;
    this->time += ms;
    // ------------------------------------------------
}

void Brick_Detector::detect(cv::Mat img)
{
    set_img(img);
    detect();
}


void Brick_Detector::clear_all()
{
    this->img.release();
    clear_predictions();
}

void Brick_Detector::find_lines()
{
    cv::Mat edge_img;
    //cv::imshow("src image", this->img);
    find_edges(this->img, edge_img);
    cv::rectangle(edge_img, cv::Point(0, 0), cv::Point(50, edge_img.size().height - 1), 0, -1); //mask out pallet
    //cv::imshow("edge_img", edge_img);
    //cv::waitKey(0);
    this->hough = Hough_space(edge_img);

    this->lines =  hough.find_lines();
    //std::cout << this->lines.size() << std::endl;
}

void Brick_Detector::find_edges(cv::Mat &src, cv::Mat &dst)
{
    cv::Mat gray_img;
    cv::cvtColor(src, gray_img, CV_BGR2GRAY);

    cv::Mat filter_img;
    cv::medianBlur(gray_img, filter_img, 5);
    //cv::GaussianBlur(gray_img, filter_img, cv::Size(3, 3), 0, 0);
    cv::Canny(filter_img, dst, this->canny_thres_low, this->canny_thres_high);
    cv::imshow("edge image", dst);
    cv::waitKey(0);
}

void Brick_Detector::find_BB(std::vector<std::vector<std::vector<double>>> clustered_lines)
{
    std::vector<std::vector<std::vector<double>>> sorted_lines;
    std::vector<std::vector<cv::Point2f>> intersection_matrix;

    if(clustered_lines[0].size() > 0 && clustered_lines[1].size() > 0)
    {
        sorted_lines = {sort_lines(clustered_lines[0]), sort_lines(clustered_lines[1])};
        intersection_matrix = find_intersection_matrix(sorted_lines);
    }

    convert_intections_to_BB(intersection_matrix);
}

std::vector<std::vector<std::vector<double>>> Brick_Detector::cluster_lines()
{
    std::vector<std::vector<double>> lines_cluster1;
    std::vector<std::vector<double>> lines_cluster2;

    float theta_ref = this->lines[0][1];
    lines_cluster1.push_back(lines[0]);

    for(unsigned int i = 1; i < lines.size(); i++)
    {
        double theta = lines[i][1];
        double diff_theta = abs(theta -  theta_ref);
        double diff = std::min(diff_theta, CV_PI-diff_theta);

        if(diff < CV_PI/4.0)
        {
            lines_cluster1.push_back(lines[i]);
        }
        else
        {
            lines_cluster2.push_back(lines[i]);
        }
    }

    return {lines_cluster1, lines_cluster2};
}

std::vector<std::vector<double> > Brick_Detector::sort_lines(std::vector<std::vector<double> > lines)
{
    std::vector<std::vector<double>> copy_lines = lines;
    double split_angle = this->hough.split_angle;
    //Method of sorting - specified by the lambda function declared below.
    std::sort(copy_lines.begin(), copy_lines.end(),
              [split_angle](const std::vector<double>& a, const std::vector<double>& b)
                {   //Lambda function
                    double rho_a = a[0];
                    if(a[1] > split_angle)
                    {
                        rho_a = -rho_a;
                    }

                    double  rho_b = b[0];
                    if(b[1] > split_angle) //TO DO: threshold should be changed before deploying.
                    {
                        rho_b = -rho_b;
                    }

                    return rho_a < rho_b;
                });

    return copy_lines;

}

std::vector<std::vector<cv::Point2f> > Brick_Detector::find_intersection_matrix(std::vector<std::vector<std::vector<double>>> clustered_lines_sorted)
{
    std::vector<std::vector<cv::Point2f>> intersection_matrix;

    for(unsigned int i = 0; i < clustered_lines_sorted[0].size(); i++)
    {
        intersection_matrix.push_back({});
        for(unsigned int j = 0; j < clustered_lines_sorted[1].size(); j++)
        {
            cv::Point2f intersection = find_intersection(clustered_lines_sorted[0][i], clustered_lines_sorted[1][j]);
            intersection_matrix[i].push_back(intersection);
        }
    }
    this->intersection_mat = intersection_matrix;
    return intersection_matrix;
}

cv::Point2f Brick_Detector::find_intersection(std::vector<double> line1, std::vector<double> line2)
{
    std::vector<cv::Point2f> points_line1 = get_points_on_line(line1);
    std::vector<cv::Point2f> points_line2 = get_points_on_line(line2);

    cv::Point2f x = points_line2[0] - points_line1[0];
    cv::Point2f d1 = points_line1[1] - points_line1[0];
    cv::Point2f d2 = points_line2[1] - points_line2[0];

    float cross = d1.x*d2.y - d1.y*d2.x;

    if (abs(cross) < /*EPS*/1e-8)
    {
        //TO DO:
        //determine what to do, if no intersection is present.
        //Should not be a problem, if no false lines is detected, but it would be nice if it was handeled.
    }

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    cv::Point2f intersection = points_line1[0] + d1 * t1;

    return intersection;
}

std::vector<cv::Point2f> Brick_Detector::get_points_on_line(std::vector<double> line)
{
    cv::Point2f pt1, pt2;
    int rho = line[0];
    double theta = line[1];
    double a = cos(theta), b = sin(theta);
    double x0 = a * rho, y0 = b * rho;
    int size = 1000; //Increase if image size increases.
    pt1.x = x0 + size*(-b) + this->img.size().width/2;
    pt1.y = y0 + size*(a) + this->img.size().height/2;
    pt2.x = x0 - size*(-b) +  this->img.size().width/2;
    pt2.y = y0 - size*(a) + this->img.size().height/2;
    return {pt1, pt2};
}

void Brick_Detector::convert_intections_to_BB(std::vector<std::vector<cv::Point2f> > intersection_matrix)
{
    for(int i = 0; i < intersection_matrix.size() - 1; i++)
    {
        for(int j = 0; j < intersection_matrix[i].size() - 1; j++)
        {
            std::vector<cv::Point2f> contour = {intersection_matrix[i][j], intersection_matrix[i][j + 1], intersection_matrix[i + 1][j], intersection_matrix[i + 1][j + 1]};
            cv::RotatedRect BB_rotated = cv::minAreaRect(contour);

            //Evaluate if the current contour could orignate from a brick.
            if(accept_detection(BB_rotated) == true) //should be used to filter out BB which does not origin form bricks
            {
                bounding_box pred(BB_rotated);
                //cv::Rect bounding_box =  cv::boundingRect(contour);
                //pred.rect = bounding_box;
                //pred.rotated_rect = BB_rotated;

                this->predictions.push_back(pred);
            }
        }
    }
}

bool Brick_Detector::accept_detection(cv::RotatedRect rotated_BB)
{
    double min_side = std::min(rotated_BB.size.width, rotated_BB.size.height);
    double max_side = std::max(rotated_BB.size.width, rotated_BB.size.height);
    double proportion = min_side/max_side;
    double area = rotated_BB.size.width * rotated_BB.size.height;
    double margin = 0.1;

    if(min_side < 20.5 * (1 - margin) || max_side > 108.0 * (1 + margin))
    {
        return false;
    }
    else if(proportion < 0.203 * (1 - margin)|| proportion > 0.296 * (1 + margin))
    {
        return false;
    }
    else if(area < 1906.0 * (1 - margin) || area > 3260.0 * (1 + margin))
    {
        return false;
    }
    else
    {
        return true;
    }
}
