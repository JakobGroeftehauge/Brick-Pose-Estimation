#include <iostream>
#include "../Includes/Evaluator.h"

using namespace std;

int main()
{
	std::string test_name = "chamfer_OUR-CCORR_Gauss3";
	std::ofstream res, dump;

	int sp = 12;
	std::vector<double> thresholds({ 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 });
	std::vector<double> nms_thresh = { 125 };// { 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400, 425, 450, 475, 500 };// , 0.2, 0.25, 0.3, 0.35, 0.4};
	Chamfer_brick_detector detector; // hough based brick detector
	//Evaluator test_evaluator("../../03 Data/Simple Dataset", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_18-03.csv");
	Evaluator test_evaluator("../../03 Data/Dataset2_onPallet", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_OP_val_set_14-04.csv");
	test_evaluator.set_detector(&detector);
	//test_evaluator.detector->set_brick_specs(96, 21, 108, 28);
	test_evaluator.detector->set_brick_specs(139, 30, 211, 54.39);
	test_evaluator.detector->set_resolution(60, 5);

	res.open("../../03 Data/Dataset2_onPallet/" + test_name + "results.csv");
	dump.open("../../03 Data/Dataset2_onPallet/" + test_name + "dump.txt");
	res << setw(sp) << "Thresh, Prec., Rec., F1, ang. err., a. e. std., Time" << std::endl;

	for (int i = 0; i < nms_thresh.size(); i++)
	{
		test_evaluator.reset_test();
		test_evaluator.detector->set_NMS_thresh(nms_thresh[i]);
		test_evaluator.evaluate_dataset(thresholds);
		dump << "NMS Threshold: " << nms_thresh[i] << std::endl;
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
		res << nms_thresh[i] << ", "  << avgs.precision << ", " << avgs.recall << ", " << avgs.f1
			<< ", " << avgs.avg_angle_err << ", " << avgs.std_angle_err << ", " << test_evaluator.detector->time << "\n";
	}
	res.close();
	dump.close();
	return 0;
}
