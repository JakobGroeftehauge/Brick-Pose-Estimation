#include "Chamfer_brick_detector.h"

Chamfer_brick_detector::Chamfer_brick_detector()
{
	this->canny_thres_high = 70;
	this->canny_thres_low = 25;
	this->NMS_thresh = 1200;
}

Chamfer_brick_detector::Chamfer_brick_detector(cv::Mat img)
{
	this->canny_thres_high = 70;
	this->canny_thres_low = 25;
	this->NMS_thresh = 120;
	this->img = img;
}

void Chamfer_brick_detector::detect()
{
	//util::Timer timer("Detection time");
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::duration<float> duration;
	start = std::chrono::high_resolution_clock::now();
	this->predictions.clear();
	this->pred_candidates.clear();
	compute_chamfer_img();
	find_rectangle_candidates(60, 100, 110, 5);
	//std::cout << this->pred_candidates.size() << std::endl;;
	//sort list
	std::sort(this->pred_candidates.begin(), this->pred_candidates.end());
	//std::cout << "Length before IOU NMS: " << this->pred_candidates.size() << std::endl;
	apply_IOU_NMS(this->pred_candidates, 0.2, this->pred_candidates);
	//std::cout << "Length after IOU NMS: " << this->pred_candidates.size() << std::endl;
	predictions_from_candidates(this->pred_candidates, this->predictions);
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	float ms = duration.count() * 1000.0f;
	this->time += ms;
}

void Chamfer_brick_detector::detect(cv::Mat img)
{
	set_img(img);
	detect();
}

void Chamfer_brick_detector::apply_NMS(cv::Mat &matching_space, std::vector<cv::Point>& best_match_locations)
{
	//Timer timer("Apply NMS");
	cv::Mat eroded, thresholded_matching_space, local_minima, thresholded_8bit;
	erode(matching_space, eroded, cv::Mat());
	compare(matching_space, eroded, local_minima, cv::CMP_EQ);
	threshold(matching_space, thresholded_matching_space, this->NMS_thresh, 255, cv::THRESH_BINARY_INV);
	//double max, min;
	//cv::minMaxLoc(matching_space, &min, &max);
	//std::cout << "Max" << max << "min " << min << std::endl;
	//cv::imshow("thresholded matching space", thresholded_matching_space);
	//cv::minMaxLoc(thresholded_matching_space, &min, &max);
	//std::cout << "Max" << max << std::endl;
	//cv::waitKey(0);
	thresholded_matching_space.convertTo(thresholded_8bit, CV_8U);
	bitwise_and(local_minima, thresholded_8bit, local_minima);
	cv::findNonZero(local_minima, best_match_locations);
}

void Chamfer_brick_detector::compute_chamfer_img()
{
	cv::Mat edge_img;
	find_edges(this->img, edge_img);
	//cv::imshow("edge img", edge_img);
	cv::threshold(edge_img, edge_img, 127, 255, cv::THRESH_BINARY_INV);
	cv::distanceTransform(edge_img, this->chamfer_img, CV_DIST_L2, 3);
	/*cv::Mat tmp_img;
	cv::normalize(this->chamfer_img, tmp_img, 0, 1.0, cv::NORM_MINMAX, CV_32F);*/
	//cv::imshow("Distance map", tmp_img);
}

void Chamfer_brick_detector::set_NMS_thresh(double thresh)
{
	this->NMS_thresh = thresh;
}

void Chamfer_brick_detector::create_template(float scale, float angle, cv::Mat &template_img_dst, cv::RotatedRect &rect_dst)
{
	//Timer timer("create template");
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
	template_ = template_ / float(white_pixels) * 100;

	// set destination variables
	template_img_dst = template_;
	rect_dst = rect;
}

void Chamfer_brick_detector::find_rectangle_candidates(int angle_steps, float scale_min, float scale_max, int scale_steps)
{
	//util::Timer rectangle_candidate_timer("find candidates");
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
			//Timer inner_loop("Inner loop");
			create_template(scale_min + j * scale_res, i * angle_res -90, template_img, tmp_rect);

			cv::matchTemplate(this->chamfer_img, template_img, tmp_matching_space, CV_TM_CCORR); // typically 5-10 ms. CV_TM_CCORR sometimes more. Release
	/*		if (i == angle_steps / 2 + 2 && j == scale_steps / 2)
			{
				cv::Mat _tmp_template;
				cv::Mat _tmp_matching;
				cv::normalize(template_img, _tmp_template, 0, 1.0, cv::NORM_MINMAX, CV_32F);
				cv::normalize(tmp_matching_space, _tmp_matching, 0, 1.0, cv::NORM_MINMAX, CV_32F);
				cv::imshow("template", _tmp_template);
                this->model_template = _tmp_template;
				cv::imshow("matching space", _tmp_matching);
                cv::normalize(tmp_matching_space, this->matching_space_disp, 0, 255, cv::NORM_MINMAX, CV_8UC1);

			}

			match_locations.clear();
			apply_NMS(tmp_matching_space, match_locations);
			//std::cout << match_locations.size() << std::endl;
			tmp_candidates.clear();
			generate_candidates(match_locations, tmp_matching_space, tmp_rect, tmp_candidates);
			// Insert intermediate NMS on tmp_candidates here
			this->pred_candidates.insert(this->pred_candidates.end(), tmp_candidates.begin(), tmp_candidates.end());
		}
	}
}

void Chamfer_brick_detector::generate_candidates(std::vector<cv::Point>& best_match_locations, cv::Mat& matching_space, cv::RotatedRect template_rect, std::vector<prediction_candidate> &candidates_dst)
{
	//Timer timer("Generate Candidates"); // typically < 0.0005 ms
	prediction_candidate tmp_candidate;
    for (unsigned int i = 0; i < best_match_locations.size(); i++)
	{
		cv::Point2f shift = best_match_locations[i];
		tmp_candidate.distance_score = matching_space.at<float>(best_match_locations[i]);
		tmp_candidate.rotated_rect = cv::RotatedRect(template_rect.center + shift, template_rect.size, template_rect.angle);
		candidates_dst.push_back(tmp_candidate);
	}
}

void Chamfer_brick_detector::apply_IOU_NMS(const std::vector<prediction_candidate>& candidates_src, float thresh, std::vector<prediction_candidate>& candidates_dst)
{

    //util::Timer timer("Apply IOU NMS");
    int i = 0;
    unsigned int j = 0;

	std::vector<prediction_candidate> candidates_src_copy(candidates_src);


	while (i < int(candidates_src_copy.size())-1)
	{
		j = i + 1;
		while (j < candidates_src_copy.size())
		{
			if (rotated_rect_IOU(candidates_src_copy[i].rotated_rect, candidates_src_copy[j].rotated_rect) > thresh)
			{
				//std::cout << rotated_rect_IOU(candidates_src_copy[i].rotated_rect, candidates_src_copy[j].rotated_rect) << std::endl;
				candidates_src_copy.erase(candidates_src_copy.begin() +  j);
			}
			else
			{

				j++;
			}
		}
		i++;
	}
	candidates_dst = candidates_src_copy;
}

float Chamfer_brick_detector::rotated_rect_IOU(cv::RotatedRect rect1, cv::RotatedRect rect2)
{
	float intersection_area = 0.0;
	std::vector<cv::Point2f> intersection_cont;
    if (cv::INTERSECT_NONE != cv::rotatedRectangleIntersection(rect1, rect2, intersection_cont))
	{
        std::vector<cv::Point2f> hull;
        cv::convexHull(intersection_cont, hull);
        intersection_area = cv::contourArea(hull);
	}
	else
	{
		intersection_area = 0.0;
	}
	float union_area = rect1.size.area() + rect2.size.area() - intersection_area;
	return intersection_area/union_area;
}

void Chamfer_brick_detector::predictions_from_candidates(std::vector<prediction_candidate>& candidates, std::vector<prediction>& predictions)
{
	prediction tmp_pred;
    for (unsigned int i = 0; i < candidates.size(); i++)
	{
		tmp_pred.rotated_rect = candidates[i].rotated_rect;
		tmp_pred.rect = candidates[i].rotated_rect.boundingRect2f(); // maybe use the boundingRectf instead
		tmp_pred.angle_vector = cv::Point2d(std::cos(candidates[i].rotated_rect.angle / 180.0 * CV_PI), std::sin(candidates[i].rotated_rect.angle / 180.0 * CV_PI));
		predictions.push_back(tmp_pred);
	}
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
