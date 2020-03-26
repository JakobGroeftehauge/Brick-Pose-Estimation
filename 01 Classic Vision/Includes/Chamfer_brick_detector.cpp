#include "Chamfer_brick_detector.h"

Chamfer_brick_detector::Chamfer_brick_detector()
{
	this->canny_thres_high = 140;
	this->canny_thres_low = 20;
	this->NMS_thresh = 200;
}

Chamfer_brick_detector::Chamfer_brick_detector(cv::Mat img)
{
	this->canny_thres_high = 85;
	this->canny_thres_low = 25;
	this->NMS_thresh = 120;
	this->img = img;
	compute_chamfer_img();
	find_rectangle_candidates(30, 100, 110, 2);
	//sort list
	std::sort(this->pred_candidates.begin(),this->pred_candidates.end());
	/*for (int i = 0; i < this->pred_candidates.size(); i++)
	{
		std::cout << this->pred_candidates[i].distance_score << std::endl;
	}*/
	std::cout << "Length before IOU NMS: " << this->pred_candidates.size() << std::endl;
	apply_IOU_NMS(this->pred_candidates, 0.3, this->pred_candidates);
	std::cout << "Length after IOU NMS: " << this->pred_candidates.size() << std::endl;
}

void Chamfer_brick_detector::set_img(cv::Mat img)
{
    this->img = img;
}

cv::Mat Chamfer_brick_detector::create_matchingspace(int num_angles, float scale)
{
    double angle_res = CV_PI/num_angles;

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


void Chamfer_brick_detector::apply_NMS(cv::Mat &matching_space, std::vector<cv::Point>& best_match_locations)
{
	cv::Mat eroded, thresholded_matching_space, local_minima, thresholded_8bit;
	erode(matching_space, eroded, cv::Mat());
	compare(matching_space, eroded, local_minima, cv::CMP_EQ);
	threshold(matching_space, thresholded_matching_space, this->NMS_thresh, 255, cv::THRESH_BINARY_INV);
	thresholded_matching_space.convertTo(thresholded_8bit, CV_8U);
	bitwise_and(local_minima, thresholded_8bit, local_minima);
	cv::findNonZero(local_minima, best_match_locations);
}

void Chamfer_brick_detector::compute_chamfer_img()
{
	cv::Mat edge_img;
	find_edges(this->img, edge_img);
	cv::imshow("edge img", edge_img);
	cv::threshold(edge_img, edge_img, 127, 255, cv::THRESH_BINARY_INV);
	cv::distanceTransform(edge_img, this->chamfer_img, CV_DIST_L2, 3);
}




void Chamfer_brick_detector::create_template(float scale, float angle, cv::Mat &template_img_dst, cv::RotatedRect &rect_dst)
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

	//Calculate norm kind of
	int white_pixels = cv::countNonZero(template_);
	template_ = template_ / float(white_pixels)*100;

	// set destination variables
	template_img_dst = template_;
	rect_dst = rect;
}


void Chamfer_brick_detector::find_rectangle_candidates(int angle_steps, float scale_min, float scale_max, int scale_steps)
{
	float angle_res = 180.0 / angle_steps;
	float scale_res = (scale_max - scale_min) / scale_steps;
	cv::Mat template_img, tmp_matching_space;
	cv::RotatedRect tmp_rect;
	std::vector<cv::Point> match_locations;
	std::vector<prediction_candidate> tmp_candidates;
	for (int i = 0; i < angle_steps; i++)
	{
		for (int j = 0; j < scale_steps; j++)
		{
			create_template(scale_min + j * scale_res, i * angle_res -90, template_img, tmp_rect);
			cv::matchTemplate(this->chamfer_img, template_img, tmp_matching_space, CV_TM_CCORR);
			match_locations.clear();
			apply_NMS(tmp_matching_space, match_locations);
			tmp_candidates.clear();
			generate_candidates(match_locations, tmp_matching_space, tmp_rect, tmp_candidates);
			// Insert intermediate NMS on tmp_candidates here
			this->pred_candidates.insert(this->pred_candidates.end(), tmp_candidates.begin(), tmp_candidates.end());
		}
	}
}

void Chamfer_brick_detector::generate_candidates(std::vector<cv::Point>& best_match_locations, cv::Mat& matching_space, cv::RotatedRect template_rect, std::vector<prediction_candidate> &candidates_dst)
{
	prediction_candidate tmp_candidate;
	for (int i = 0; i < best_match_locations.size(); i++)
	{
		cv::Point2f shift = best_match_locations[i];
		tmp_candidate.distance_score = matching_space.at<float>(best_match_locations[i]);
		tmp_candidate.rotated_rect = cv::RotatedRect(template_rect.center + shift, template_rect.size, template_rect.angle);
		candidates_dst.push_back(tmp_candidate);
	}
}

void Chamfer_brick_detector::apply_IOU_NMS(const std::vector<prediction_candidate>& candidates_src, float thresh, std::vector<prediction_candidate>& candidates_dst)
{
	int i = 0;
	std::vector<prediction_candidate> candidates_src_copy(candidates_src);
	auto itv = candidates_src_copy.begin();
	auto itv2 = std::next(itv, 1);
	while (itv != candidates_src_copy.end())
	{
		itv2 = std::next(itv, 1);
		while (itv2 != candidates_src_copy.end())
		{
			if (itv == itv2)
				std::cout << "They are the same" << itv->distance_score << std::endl;
			std::cout << "itv: " << std::distance(candidates_src_copy.begin(), itv) << " itv2: " << std::distance(candidates_src_copy.begin(), itv2) << " Length: "<< candidates_src_copy.size() << std::endl;

			if (rotated_rect_IOU(itv->rotated_rect, itv2->rotated_rect) > thresh)
			{
				itv2 = candidates_src_copy.erase(itv2);
			}
			else
			{
				++itv2;
			}

		}
		++itv;
		i++;
	}
	candidates_dst = candidates_src_copy;
}

float Chamfer_brick_detector::rotated_rect_IOU(cv::RotatedRect rect1, cv::RotatedRect rect2)
{
	float intersection_area = 0.0;
	std::vector<cv::Point2f> intersection_cont;
	cv::rotatedRectangleIntersection(rect1, rect2, intersection_cont);
	if (intersection_cont.size() > 2)
	{
		intersection_area = cv::contourArea(intersection_cont);
	}
	float union_area = rect1.size.area() + rect2.size.area() - intersection_area;
	return intersection_area/union_area;
}





void Chamfer_brick_detector::find_edges(cv::Mat& src, cv::Mat& dst)
{
	cv::Mat gray_img;
	cv::cvtColor(src, gray_img, CV_BGR2GRAY);
	cv::Mat filter_img;
	//cv::medianBlur(gray_img, filter_img, 3);
	cv::GaussianBlur(gray_img, filter_img, cv::Size(3,3),0,0);
	cv::Canny(filter_img, dst, this->canny_thres_low, this->canny_thres_high);
}
