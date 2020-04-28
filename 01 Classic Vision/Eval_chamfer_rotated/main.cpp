#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

using namespace std;

int main()
{
    int sp = 12;
    std::vector<double> thresholds ={0.9};
    std::string test_name = "1234";
    Chamfer_brick_detector detector;
    Evaluator test_evaluator("../../03 Data/Dataset2_onPallet", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_OP_val_set_14-04.csv", NON_AXIS_ALIGNED);
    test_evaluator.set_detector(&detector);
    test_evaluator.detector->set_brick_specs(139, 30, 211, 54.39);
    test_evaluator.detector->set_resolution(2, 2);

    test_evaluator.evaluate_dataset(thresholds);

    for (int k = 0; k < test_evaluator.results.size(); k++)
    {
        evaluation_results tmp_res = test_evaluator.results[k];
        cout << setw(sp) << tmp_res.threshold << setw(sp) << tmp_res.precision << setw(sp) << tmp_res.recall << setw(sp) << tmp_res.f1 << setw(sp)
            << tmp_res.avg_angle_err << setw(sp) << tmp_res.std_angle_err << setw(sp) << tmp_res.total_FN << setw(sp)
            << tmp_res.total_FP << setw(sp) << tmp_res.total_TP << endl;
    }

    cout << "Hello World!" << endl;
    return 0;
}
