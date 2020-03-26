#include "Chamfer_brick_detector.h"

Chamfer_brick_detector::Chamfer_brick_detector()
{
	this->canny_thres_high = 140;
	this->canny_thres_low = 60;
}

Chamfer_brick_detector::Chamfer_brick_detector(cv::Mat img)
{
	this->canny_thres_high = 140;
	this->canny_thres_low = 35;
	this->img = img;
	compute_chamfer_img();
    cv::RotatedRect r_rect = create_template(200, 30);
	cv::matchTemplate(this->chamfer_img, this->model_template, this->matching_space, CV_TM_CCORR);
	std::vector<cv::Point> locations;
	apply_NMS(locations,1000);
	this->pred_candidates.empty();
	create_rot_rect_at_locations(locations, r_rect);
}

void Chamfer_brick_detector::set_img(cv::Mat img)
{
    this->img = img;
}

cv::Mat Chamfer_brick_detector::create_matchingspace(int num_angles, float scale)
{
    double angle_res = M_PI/num_angles;

    int match_width = img.cols - brick_template.size.width * scale - template_padding * 2;
    int match_heigt = img.rows - brick_template.size.height * scale - template_padding * 2;

    std::vector<int> dims = {match_width, match_heigt, num_angles};  //macth_height and
    cv::Mat stacked_matching_space(dims, CV_32F);

    for(int i = 0; i < num_angles; i++)
    {
         std::cout << " height " << match_heigt << " width "<< match_width << std::endl;
        cv::Mat match_space = cv::Mat::ones(match_width, match_heigt, CV_32F);
        std::cout << "Dim 0 " << match_space.size[0] << " Dim 1 " << match_space.size[1] << std::endl;
        int mapping_channels[] = {0,0 , 1,1, 2,i};
        mixChannels(&match_space, 1, &stacked_matching_space, num_angles, mapping_channels, 4);
    }

    return stacked_matching_space;
}


void Chamfer_brick_detector::apply_NMS(std::vector<cv::Point>& dst_locations, float thresh)
{
	cv::Mat eroded, thresholded_matching_space, local_minima, thresholded_8bit;
	erode(this->matching_space, eroded, cv::Mat());
	compare(this->matching_space, eroded, local_minima, cv::CMP_EQ);
	threshold(this->matching_space, thresholded_matching_space, thresh, 255, cv::THRESH_BINARY_INV);
	thresholded_matching_space.convertTo(thresholded_8bit, CV_8U);
	bitwise_and(local_minima, thresholded_8bit, local_minima);
	cv::findNonZero(local_minima, dst_locations);
}

void Chamfer_brick_detector::compute_chamfer_img()
{
	cv::Mat edge_img;
	find_edges(this->img, edge_img);
	cv::imshow("edge img", edge_img);
	cv::threshold(edge_img, edge_img, 127, 255, cv::THRESH_BINARY_INV);
	cv::distanceTransform(edge_img, this->chamfer_img, CV_DIST_L2, 3);
}




cv::RotatedRect Chamfer_brick_detector::create_template(float scale, float angle)
{
    // Scale and shift brick template
    cv::Mat template_ = cv::Mat::zeros(scale + template_padding * 2, scale + template_padding * 2, CV_32F); // add zeropixels around the circumfrance of the template.
    cv::Point2f shifted_center_point = cv::Point2f(this->brick_template.center.x * scale + template_padding, this->brick_template.center.y * scale + template_padding);
    cv::Size2f scaled_size = cv::Size2f(this->brick_template.size.width * scale, this->brick_template.size.height * scale);
    cv::RotatedRect rect(shifted_center_point, scaled_size, angle);

    // Draw rotated rect
    cv::Point2f vertices[4];
    rect.points(vertices);
    for(int i = 0; i < 4; i++)
    {
        line(template_, vertices[i], vertices[(i + 1) % 4], cv::Scalar(1.0), 2);
    }

    template_ = template_ * 0.5;
    this->model_template = template_;

    return rect;

}

void Chamfer_brick_detector::create_rot_rect_at_locations(std::vector<cv::Point> &locations, cv::RotatedRect template_rect)
{
	cv::Point shift = template_rect.center;
	prediction_candidate candidate;
	for (int i = 0; i < locations.size(); i++)
	{
		candidate.distance_score = this->matching_space.at<float>(locations[i]);
		candidate.rotated_rect = cv::RotatedRect(locations[i] + shift, template_rect.size, template_rect.angle);
		this->pred_candidates.push_back(candidate);
    }
}


void Chamfer_brick_detector::find_edges(cv::Mat& src, cv::Mat& dst)
{
	cv::Mat gray_img;
	cv::cvtColor(src, gray_img, CV_BGR2GRAY);
	cv::Mat filter_img;
	cv::medianBlur(gray_img, filter_img, 5);
	cv::Canny(filter_img, dst, this->canny_thres_low, this->canny_thres_high);
}
