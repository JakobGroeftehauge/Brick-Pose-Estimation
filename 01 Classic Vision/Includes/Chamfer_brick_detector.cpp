#include "Chamfer_brick_detector.h"

Chamfer_brick_detector::Chamfer_brick_detector()
{
	this->canny_thres_high = 140;
	this->canny_thres_low = 60;
}

Chamfer_brick_detector::Chamfer_brick_detector(cv::Mat img)
{
	this->canny_thres_high = 140;
	this->canny_thres_low = 60;
	this->img = img;
	compute_chamfer_img();
	create_template(1, 9);
	ChamferMatching(this->chamfer_img, this->model_template, this->matching_space);
}

void Chamfer_brick_detector::set_img(cv::Mat img)
{
	this->img = img;
}

void Chamfer_brick_detector::ChamferMatching(cv::Mat& chamfer_image, cv::Mat& model, cv::Mat& matching_image)
{
	// Extract the model points (as they are sparse). 
	std::vector<cv::Point> model_points; 
	int image_channels = model.channels(); 
	for (int model_row=0; (model_row < model.rows); model_row++) 
	{ 
		uchar *curr_point = model.ptr<uchar>(model_row); 
		for (int model_column=0; (model_column<model.cols); model_column++) 
		{ 
			if (*curr_point > 0) 
			{ 
				//cv::Point& new_point = cv::Point(model_column,model_row);
				model_points.push_back(cv::Point(model_column, model_row));
			} 
			curr_point += image_channels;
		}
	} int num_model_points = model_points.size(); 
	image_channels = chamfer_image.channels(); 
	// Try the model in every possible position 
	matching_image = cv::Mat(chamfer_image.rows-model.rows+1, chamfer_image.cols-model.cols+1, CV_32FC1); 
	for (int search_row=0; (search_row <= chamfer_image.rows-model.rows); search_row++) 
	{ 
		float *output_point = matching_image.ptr<float>(search_row); 
		for (int search_column=0; (search_column <= chamfer_image.cols-model.cols); search_column++) 
		{ 
			float matching_score = 0.0; 
			for (int point_count = 0; (point_count < num_model_points); point_count++)
			{
				matching_score += (float)*(chamfer_image.ptr<float>(model_points[point_count].y + search_row) + search_column + model_points[point_count].x * image_channels);
			}
				*output_point = matching_score; 
				output_point++; 
		} 
	}

}

void Chamfer_brick_detector::compute_chamfer_img()
{
	cv::Mat edge_img;
	find_edges(this->img, edge_img);

	cv::threshold(edge_img, edge_img, 127, 255, cv::THRESH_BINARY_INV);

	cv::distanceTransform(edge_img, edge_img, CV_DIST_L2, 3);
	//cv::normalize(edge_img, edge_img, 0, 1.0, cv::NORM_MINMAX);
	//cv::imshow("test2", edge_img);
	
	this->chamfer_img = edge_img;
}

void Chamfer_brick_detector::create_template(float scale, float angle)
{
	// mean width: 100.9004, mean height: 24.5373
	// ratio: 4.1214:1
	float width = 100.9004, height = 24.5373;
	float diag = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
	this->model_template = cv::Mat::zeros(int(diag) + 7, int(diag) + 7, CV_8U);
	cv::RotatedRect rect(cv::Point2f(this->model_template.rows / 2, this->model_template.cols / 2),cv::Point2f(width, height),angle);
	cv::Point2f vertices[4];
	rect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(this->model_template, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255), 2);
}

void Chamfer_brick_detector::find_edges(cv::Mat& src, cv::Mat& dst)
{
	cv::Mat gray_img;
	cv::cvtColor(src, gray_img, CV_BGR2GRAY);

	cv::Mat filter_img;
	cv::medianBlur(gray_img, filter_img, 5);

	cv::Canny(filter_img, dst, this->canny_thres_low, this->canny_thres_high);
}
