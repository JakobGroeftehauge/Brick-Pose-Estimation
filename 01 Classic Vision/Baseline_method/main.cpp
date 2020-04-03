#include <iostream>
#include "../Includes/Evaluator.h"

using namespace std;

void run_evaluation(double NMS_thresh) {
    int spacing = 17; // spacing for table print at the end
    std::vector<double> thresholds({ 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 });
    std::vector<double> total_false_negatives(thresholds.size(), 0);
    std::vector<double> total_false_positives(thresholds.size(), 0);
    std::vector<double> total_true_positives(thresholds.size(), 0);
    Chamfer_brick_detector detector;
    Evaluator test_evaluator("../../03 Data/Simple Dataset", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_18-03.csv");
    test_evaluator.set_detector(&detector);
    test_evaluator.set_thresholds(thresholds);
    test_evaluator.reset_counters();
    test_evaluator.detector->set_NMS_thresh(NMS_thresh);
    while (test_evaluator.evaluate_next_img())
    {
        for (int i = 0; i < thresholds.size(); i++)
        {
            total_false_negatives[i] += test_evaluator.false_negative_range[i];
            total_false_positives[i] += test_evaluator.false_positive_range[i];
            total_true_positives[i] += test_evaluator.true_positive_range[i];
            //std::cout << "False negative: " << test_evaluator.false_negative_range[i] << " False Positive: "
              //  << test_evaluator.false_positive_range[i] << " True Positive: " << test_evaluator.true_positive_range[i] << std::endl;
        }
    }
    std::cout << "Evaluation at NMS threshold: " << NMS_thresh << std::endl;
    std::cout << setw(5) << "IoU" << setw(spacing) << "Precision" << setw(spacing) << "Recall" << setw(spacing) << "F1" << setw(spacing)
        << "False Negatives" << setw(spacing) << "False Positives" << setw(spacing) << "True Posiives" << std::endl;
    double avg_precision = 0, avg_recall = 0, avg_f1 = 0;
    for (int i = 0; i < thresholds.size(); i++)
    {
        double precision = total_true_positives[i] / (total_true_positives[i] + total_false_positives[i]);
        double recall = total_true_positives[i] / (total_true_positives[i] + total_false_negatives[i]);
        double f1 = (2 * recall * precision) / (recall + precision);
        std::cout << setw(5) << thresholds[i] << setw(spacing) << precision << setw(spacing) << recall << setw(spacing) << f1 << setw(spacing)
            << total_false_negatives[i] << setw(spacing) << total_false_positives[i] << setw(spacing) << total_true_positives[i] << std::endl;
        avg_precision += precision / thresholds.size();
        avg_recall += recall / thresholds.size();
        avg_f1 += f1 / thresholds.size();
    }
    std::cout << setw(5) << "Avg" << setw(spacing) << avg_precision << setw(spacing) << avg_recall << setw(spacing) << avg_f1 << setw(spacing)
        << "--" << setw(spacing) << "--" << setw(spacing) << "--" << std::endl;
    std::cout << "Detection time: " << test_evaluator.detector->time << std::endl;
}

int main()
{
    double steps = 20;
    double min = 0;
    double max = 500;//169942;
    double step_size = (max - min) / steps;
    for (int i = 1; i <= steps; i++)
    {
        run_evaluation(i * step_size);
        std::cout << "--------------------------------------------------------------------------------- \n" << std::endl;
    }

    return 0;
}

// What we want
// for one threshold value, calculate the average f1 score for different thresholds.
    // to save time, calculate the whole array of f1 at once.
