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
	cv::RotatedRect r_rect = create_template(1, 9);
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
	// mean width: 100.9004, mean height: 24.5373
	// ratio: 4.1214:1
	float width = 100.9004, height = 24.5373;
	float diag = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
	this->model_template = cv::Mat::zeros(int(diag) + 7, int(diag) + 7, CV_32F);
	cv::RotatedRect rect(cv::Point2f(this->model_template.rows / 2, this->model_template.cols / 2),cv::Point2f(width, height),angle);
	cv::Point2f vertices[4];
	rect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(this->model_template, vertices[i], vertices[(i + 1) % 4], cv::Scalar(1.0), 2);
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
