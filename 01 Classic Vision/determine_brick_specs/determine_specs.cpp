#include <iostream>
#include <vector>
#include <numeric>
#include "../Includes/data_loader.h"

int main()
{
	std::vector<double> areas, width, height, ratio;
	Data_loader loader("../../03 Data/Simple Dataset", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_18-03.csv");
	while (loader.loadNext())
	{
		for (int i = 0; i < loader.annotations.size(); i++)
		{
			areas.push_back(loader.annotations[i].rotated_rect.size.area());
			width.push_back(loader.annotations[i].rotated_rect.size.width);
			height.push_back(loader.annotations[i].rotated_rect.size.height);
			ratio.push_back(loader.annotations[i].rotated_rect.size.height / loader.annotations[i].rotated_rect.size.width);
		}
	}
	double min_area = *std::min_element(areas.begin(), areas.end());
	double max_area = *std::max_element(areas.begin(), areas.end());
	double avg_area = accumulate(areas.begin(), areas.end(),0.0)/areas.size();

	double min_width = *std::min_element(width.begin(), width.end());
	double max_width = *std::max_element(width.begin(), width.end());
	double avg_width = std::accumulate(width.begin(), width.end(),0.0)/width.size();

	double min_height = *std::min_element(height.begin(), height.end());
	double max_height = *std::max_element(height.begin(), height.end());
	double avg_height = std::accumulate(height.begin(), height.end(),0.0)/height.size();


	double min_ratio = *std::min_element(ratio.begin(), ratio.end());
	double max_ratio = *std::max_element(ratio.begin(), ratio.end());
	double avg_ratio = std::accumulate(ratio.begin(), ratio.end(),0.0)/ratio.size();

	std::cout << "Min area: " << min_area << " max area: " << max_area << " avg area: " << avg_area<< std::endl;
	std::cout << "Min width: " << min_width << " max width: " << max_width << " avg width: " << avg_width << std::endl;
	std::cout << "Min height: " << min_height << " max height: " << max_height << " avg height: " << avg_height << std::endl;
	std::cout << "Min ratio: " << min_ratio << " max ratio: " << max_ratio << " avg ratio: " << avg_ratio << std::endl;

	return 0;
}