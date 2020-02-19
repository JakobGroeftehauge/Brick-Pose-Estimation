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

double Evaluator::calculate_IoU(cv::Rect rect1, cv::Rect rect2)
{
	return (rect1 & rect2).area() / (rect1 | rect2).area();
}
