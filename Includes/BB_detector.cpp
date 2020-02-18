#include "BB_detector.h"

BB_detector::BB_detector()
{
    //should maybe be set smarter!
    this->img_height = 400;
    this->img_width = 400;
}



void BB_detector::detect_BB(vector<vector<double>> lines)
{
    //Delete old predictions.
    this->BB = {};
    this->rotated_BB = {};

    //The Class could possible invoke the images itself.

    vector<vector<vector<double>>> clustered_lines = cluster_lines(lines);
    //sort
    if(clustered_lines[0].size() > 0 & clustered_lines[1].size() > 0)
    {
    vector<vector<vector<double>>> sorted_lines = {sort_lines(clustered_lines[0]), sort_lines(clustered_lines[1])};
    vector<vector<cv::Point2f>> intersection_matrix = get_intersection_matrix(sorted_lines);

    get_bounding_boxes(intersection_matrix);
    }
}

vector<vector<vector<double>>> BB_detector::cluster_lines(vector<vector<double> > lines)
{
    vector<vector<double>> lines_cluster1;
    vector<vector<double>> lines_cluster2;

    float theta_ref = lines[0][1];
    lines_cluster1.push_back(lines[0]);

    for(unsigned int i = 1; i < lines.size(); i++)
    {
        double theta = lines[i][1];
        double diff_theta = abs(theta -  theta_ref);
        double diff = min(diff_theta, CV_PI-diff_theta);

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

vector<vector<double>> BB_detector::sort_lines(vector<vector<double> > lines)
{
    vector<vector<double>> copy_lines = lines;
    //Bubble sort
    //bool swapped;
    //int count = 0;

    // find general angle

    double general_angle = lines[0][0];
    std::sort(copy_lines.begin(), copy_lines.end(),
              [](const std::vector<double>& a, const std::vector<double>& b) {

        double rho_a = a[0];
        if(a[1] > 2.5)
        {
            rho_a = -rho_a;
        }

        double  rho_b = b[0];
        if(b[1] > 2.5)
        {
            rho_b = -rho_b;
        }

        return rho_a < rho_b;

   /*   double theta_a = a[1];
      double rho_a = a[0];

      if(rho_a < 0)
      {
          theta_a = CV_PI - theta_a;
      }
      double a_point_x = cos(theta_a) * rho_a;
      double a_point_y = sin(theta_a) * rho_a;
      double a_x_comp =  cos(CV_PI-(CV_PI/2.0-theta_a)) * a_point_x - sin(CV_PI-(CV_PI/2.0-theta_a)) * a_point_y;



      double theta_b = b[1];
      double rho_b = b[0];
      if(rho_b < 0)
      {
          theta_b = CV_PI - theta_b;
      }

      double b_origin_x = cos(theta_b) * rho_b;
      double b_origin_y = sin(theta_b) * rho_b;
      double b_x_comp =  cos(CV_PI-(CV_PI/2.0-theta_b)) * b_origin_x - sin(CV_PI-(CV_PI/2.0-theta_b)) * b_origin_y;

      return a_x_comp < b_x_comp;*/
    });

    return copy_lines;
}

vector<cv::Point2f> BB_detector::get_points_on_line(vector<double> line)
{
    cv::Point2f pt1, pt2;
    int rho = line[0];
    double theta = line[1];
    double a = cos(theta), b = sin(theta);
    double x0 = a * rho, y0 = b * rho;
    int size = 1000; //Increase if image size increases.
    pt1.x = x0 + size*(-b) + this->img_width/2;
    pt1.y = y0 + size*(a) + this->img_height/2;
    pt2.x = x0 - size*(-b) + this->img_width/2;
    pt2.y = y0 - size*(a) + this->img_height/2;
    return {pt1, pt2};
}

cv::Point2f BB_detector::get_intersection(vector<double> line1, vector<double> line2)
{
    vector<cv::Point2f> points_line1 = get_points_on_line(line1);
    vector<cv::Point2f> points_line2 = get_points_on_line(line2);

    cv::Point2f x = points_line2[0] - points_line1[0];
    cv::Point2f d1 = points_line1[1] - points_line1[0];
    cv::Point2f d2 = points_line2[1] - points_line2[0];

    float cross = d1.x*d2.y - d1.y*d2.x;

    if (abs(cross) < /*EPS*/1e-8)
    {
        //determine what to do, if no intersection is present.
    }

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    cv::Point2f intersection = points_line1[0] + d1 * t1;

    return intersection;
}

vector<vector<cv::Point2f> > BB_detector::get_intersection_matrix(vector<vector<vector<double> > > clustered_lines)
{
    vector<vector<cv::Point2f>> intersection_matrix;

    for(unsigned int i = 0; i < clustered_lines[0].size(); i++)
    {
        intersection_matrix.push_back({});
        for(unsigned int j = 0; j < clustered_lines[1].size(); j++)
        {
            cv::Point2f intersection = get_intersection(clustered_lines[0][i], clustered_lines[1][j]);
            intersection_matrix[i].push_back(intersection);
        }
    }

    return intersection_matrix;
}

bool BB_detector::point_within_boundaries(cv::Point2f point)
{
    if(point.x < this->img_width && point.x > 0 && point.y < this->img_height && point.y > 0)
    {
        return true;
    }

    return false;
}

void BB_detector::get_bounding_boxes(vector<vector<cv::Point2f> > intersection_matrix)
{
    for(unsigned int i = 0; i < intersection_matrix.size() - 1; i++)
    {
        for(unsigned int j = 0; j < intersection_matrix[i].size() - 1; j++)
        {
            vector<cv::Point2f> contour = {intersection_matrix[i][j], intersection_matrix[i][j + 1], intersection_matrix[i + 1][j], intersection_matrix[i + 1][j + 1]};
            cv::RotatedRect BB_rotated = cv::minAreaRect(contour);

            //Evaluate if the current contour could orignate from a brick.
            if(accept_detection(BB_rotated) == true)
            {
                cv::Rect bounding_box =  cv::boundingRect(contour);
                this->BB.push_back(bounding_box);
                this->rotated_BB.push_back(BB_rotated);
            }
        }
    }
}

bool BB_detector::accept_detection(cv::RotatedRect rotated_BB)
{
    return true;
}
