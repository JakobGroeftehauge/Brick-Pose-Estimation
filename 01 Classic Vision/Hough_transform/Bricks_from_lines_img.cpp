#include "Bricks_from_lines_img.h"

Bricks_from_lines_img::Bricks_from_lines_img(cv::Mat src_img, std::vector<std::vector<double>> lines)
{
	this->rng(1234567);
	this->base_img = cv::Mat(src_img.size().height, src_img.size().width, CV_8U) * 255;
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        print_line_bw(this->base_img, lines[i][0], lines[i][1]);
    }
    cv::rectangle(this->base_img, cv::Point(0, 0), cv::Point(this->base_img.size().width - 1, this->base_img.size().height - 1), 255, 2);
	cv::connectedComponents(this->base_img, this->base_img, 4, CV_16U);
	this->base_img.convertTo(this->base_img, CV_8U);
	cv::threshold(this->base_img, this->base_img, 1, 255, cv::THRESH_TOZERO);
	findContours(this->base_img, this->contours, this->hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	//save_to_csv(labeled_image, "labeled");
	//cv::normalize(this->base_img, this->base_img, 0, 255, cv::NORM_MINMAX, CV_8U);
	for (int i = 0; i < this->contours.size(); i++)
	{
		this->rotated_rectangles.push_back(minAreaRect(contours[i]));
	}

}

void Bricks_from_lines_img::draw_contours(cv::Mat img)
{
	for (size_t i = 0; i < this->contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(this->rng.uniform(0, 256), this->rng.uniform(0, 256), this->rng.uniform(0, 256));
		drawContours(img, this->contours, (int)i, color, 2, cv::LINE_8, this->hierarchy, 0);
	}
}

void Bricks_from_lines_img::draw_rotated_rect(cv::Mat img)
{
	for (int i = 0; i < this->rotated_rectangles.size(); i++)
	{
		cv::Point2f vertices[4];
		this->rotated_rectangles[i].points(vertices);
		for (int i = 0; i < 4; i++)
			line(img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 255, 0), 1);
	}
}

void Bricks_from_lines_img::draw_rect(cv::Mat img)
{
	for (int i = 0; i < this->contours.size(); i++)
	{
		cv::Rect brect = boundingRect(contours[i]);
		rectangle(img, brect, cv::Scalar(255, 0, 0), 2);
	}

}

std::vector<std::vector<cv::Point>> Bricks_from_lines_img::get_contours()
{
	return this->contours;
}

/*
* Used for printing the black lines which create the areas to find contours around.
*/
void Bricks_from_lines_img::print_line_bw(cv::Mat img, double rho, double theta)
{
	{
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		int size = 1000;//max(img.size().height, img.size().width) * 2; //just needs to be big enough.
		pt1.x = cvRound(x0 + size * (-b)) + img.size().width / 2;
		pt1.y = cvRound(y0 + size * (a)) + img.size().height / 2;
		pt2.x = cvRound(x0 - size * (-b)) + img.size().width / 2;
		pt2.y = cvRound(y0 - size * (a)) + img.size().height / 2;
		line(img, pt1, pt2, 0, 2);
	}
}
