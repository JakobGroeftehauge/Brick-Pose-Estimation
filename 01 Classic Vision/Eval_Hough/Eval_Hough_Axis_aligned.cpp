#include <iostream>
#include "../Includes/Evaluator.h"

using namespace std;

int main()
{
	int sp = 12;
	std::vector<double> thresholds({ 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 });
	Brick_Detector detector; // hough based brick detector
    Evaluator test_evaluator("../../03 Data/Simple Dataset Copied", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_18-03.csv");
	test_evaluator.set_detector(&detector);
	test_evaluator.detector->set_canny_thresh(51, 166);
	test_evaluator.evaluate_dataset(thresholds);
	for (int i = 0; i < test_evaluator.results.size(); i++)
	{
		evaluation_results tmp_res = test_evaluator.results[i];
		std::cout << setw(sp) << tmp_res.threshold << setw(sp) << tmp_res.precision << setw(sp) << tmp_res.recall << setw(sp) << tmp_res.f1 << setw(sp)
			<< tmp_res.avg_angle_err << setw(sp) << tmp_res.std_angle_err << setw(sp) << tmp_res.total_FN << setw(sp)
			<< tmp_res.total_FP << setw(sp) << tmp_res.total_TP << endl;
	}

	return 0;
}
