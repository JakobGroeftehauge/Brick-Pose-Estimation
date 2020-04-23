#include <iostream>
#include "../Includes/Evaluator.h"

using namespace std;

int main()
{
	std::string test_name = "canny_val_med7_mask_OP";
	std::ofstream res, dump;

	int sp = 12;
	std::vector<double> thresholds({ 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 });
	Brick_Detector detector; // hough based brick detector
    //Evaluator test_evaluator("../../03 Data/Simple Dataset", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_18-03.csv");
    Evaluator test_evaluator("../../03 Data/Dataset2_onPallet", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_OP_val_set_14-04.csv");
	
	test_evaluator.set_detector(&detector);
	//test_evaluator.detector->set_brick_specs(96, 21, 108,28);
	test_evaluator.detector->set_brick_specs(139, 30, 211,54.39);

	std::vector<int> canny_lows = { 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180 };//{ 36,39,42,45,48,51,54,57,60,63,66,69,72,75 };//
	std::vector<int> canny_hysts = { 15, 30, 45,  60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225 };//{ 60,63,66,69,72,75,78,81,84,87,90,93,96,99,102,105,108,111,114,117,120,123,126,129,132,135 };//
	//std::vector<int> canny_lows = { 36,39,42,45,48,51,54,57,60,63,66,69,72,75 };//
	//std::vector<int> canny_hysts = { 60,63,66,69,72,75,78,81,84,87,90,93,96,99,102,105,108,111,114,117,120,123,126,129,132,135 };//


	res.open("../../03 Data/Dataset2_onPallet/" + test_name + "results.csv");
	dump.open("../../03 Data/Dataset2_onPallet/" + test_name + "dump.txt");
	res << "low/hyst";
	for (int i = 0; i < canny_hysts.size(); i++) // create the header of the table in result file
	{
		res << ", " << canny_hysts[i];
	}
	for (int i = 0; i < canny_lows.size(); i++)
	{
		res << "\n" << canny_lows[i];
		for (int j = 0; j < canny_hysts.size(); j++)
		{
			int canny_low = canny_lows[i];
			int canny_high = canny_lows[i] + canny_hysts[j];
			test_evaluator.detector->set_canny_thresh(canny_low, canny_high);
			test_evaluator.reset_test();
			test_evaluator.evaluate_dataset(thresholds);
			dump << "Threshold low: " << canny_low << " Threshold high: " << canny_high << " Hyst: " << canny_hysts[j] << std::endl;
			dump << setw(sp) << "IoU" << setw(sp) << "Prec." << setw(sp) << "Rec." << setw(sp) << "F1" << setw(sp)
				<< "ang. err." << setw(sp) << "a. e. std." << setw(sp) << "F. Neg" << setw(sp) << "F. Pos" << setw(sp) << "T. Pos" << std::endl;
			for (int k = 0; k < test_evaluator.results.size(); k++)
			{
				evaluation_results tmp_res = test_evaluator.results[k];
				dump << setw(sp) << tmp_res.threshold << setw(sp) << tmp_res.precision << setw(sp) << tmp_res.recall << setw(sp) << tmp_res.f1 << setw(sp)
					<< tmp_res.avg_angle_err << setw(sp) << tmp_res.std_angle_err << setw(sp) << tmp_res.total_FN << setw(sp)
					<< tmp_res.total_FP << setw(sp) << tmp_res.total_TP << endl;
			}
			evaluation_results avgs = test_evaluator.get_avg_result();
			dump << setw(sp) << "Avg." << setw(sp) << avgs.precision << setw(sp) << avgs.recall << setw(sp) << avgs.f1 << setw(sp)
				<< avgs.avg_angle_err << setw(sp) << avgs.std_angle_err << setw(sp) << avgs.total_FN << setw(sp)
				<< avgs.total_FP << setw(sp) << avgs.total_TP << endl;
			dump << "Time " << test_evaluator.detector->time << std::endl;
			res << ", " << avgs.f1;
		}
	}
	res.close();
	dump.close();
	return 0;
}
