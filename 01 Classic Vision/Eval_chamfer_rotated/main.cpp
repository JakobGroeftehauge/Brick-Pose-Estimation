#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

using namespace std;

int main()
{
    int sp = 12;
    std::vector<double> thresholds ={0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95};
    std::ofstream res;
    std::string test_name = "res.txt";
    Chamfer_brick_detector detector;
    Evaluator test_evaluator("../../03 Data/Dataset2_onPallet", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_OP_val_set_14-04.csv", NON_AXIS_ALIGNED);
    test_evaluator.set_detector(&detector);
    test_evaluator.detector->set_brick_specs(139, 30, 211, 54.39);
    test_evaluator.detector->set_resolution(60, 5);

    test_evaluator.evaluate_dataset(thresholds);
    test_evaluator.detector->set_NMS_thresh(175);
    res.open(test_name);

    res << setw(sp) << "IoU" << setw(sp) << "Prec." << setw(sp) << "Rec." << setw(sp) << "F1" << setw(sp)
                << "ang. err." << setw(sp) << "a. e. std." << setw(sp) << "F. Neg" << setw(sp) << "F. Pos" << setw(sp) << "T. Pos" << std::endl;
    for (int k = 0; k < test_evaluator.results.size(); k++)
    {
        evaluation_results tmp_res = test_evaluator.results[k];
        res << setw(sp) << tmp_res.threshold << setw(sp) << tmp_res.precision << setw(sp) << tmp_res.recall << setw(sp) << tmp_res.f1 << setw(sp)
            << tmp_res.avg_angle_err << setw(sp) << tmp_res.std_angle_err << setw(sp) << tmp_res.total_FN << setw(sp)
            << tmp_res.total_FP << setw(sp) << tmp_res.total_TP << endl;
    }

    cout << "Hello World!" << endl;
    return 0;
}
