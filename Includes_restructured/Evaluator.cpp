#include "Evaluator.h"

Evaluator::Evaluator()
{
}

Evaluator::Evaluator(std::string path)
{
	set_path(path);
}

void Evaluator::set_path(std::string path)
{
	this->loader = Data_loader(path);
}

bool Evaluator::evaluate_next_img(double threshold = 0.5)
{
	if (loader.loadNext())
	{
		this->detector.detect(loader.img);

		return true;
	}
	else
	{
		std::cout << "No more images to evaluate" << std::endl;
		return false;
	}
}

double Evaluator::calculate_IoU(cv::Rect rect1, cv::Rect rect2)
{
	return (rect1 & rect2).area() / (rect1 | rect2).area();
}

void Evaluator::evaluate(double threshold = 0.5)
{
	std::vector<cv::Rect> annotations(this->loader.Bounding_boxes);
	std::vector<cv::Rect> predictions;
	for (int i = 0; i < detector.predictions.size(); i++)
	{
		predictions.push_back(detector.predictions[i].rect);
	}

}
