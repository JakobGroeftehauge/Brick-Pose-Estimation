#include <iostream>
#include "../Includes/Evaluator.h"
//#include "../Includes/util.h"
//#include "Brick_Detector.h"


using namespace std;

int main()
{
    //Brick_Detector detector;
    //test_evaluator.open_file();
    //test_evaluator.close_file();

    std::vector<double> test({ 0.5 });
    std::cout << test[0] << std::endl;
    Data_loader tmp_loader("../../03 Data/Simple Dataset", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_18-03.csv");
    double IOU_pct[] = { 0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95 };
    std::vector<double> thresholds({0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95});
    std::vector<double> total_false_negatives(thresholds.size(), 0);
    std::vector<double> total_false_positives(thresholds.size(), 0);
    std::vector<double> total_true_positives(thresholds.size(), 0);
    Chamfer_brick_detector detector;
    //Evaluator test_evaluator("../../03 Data/Simple Dataset", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_18-03.csv");
    //test_evaluator.set_detector(&detector);
    //test_evaluator.set_thresholds(thresholds);
    //test_evaluator.reset_counters();
    double glob_min = 1000;
    double glob_max = 0;
    
    double min;
    double max;

    while (0)//tmp_loader.loadNext())
    {
        detector.set_img(tmp_loader.img);
        detector.compute_chamfer_img();
        cv::minMaxLoc(detector.chamfer_img, &min, &max);
        if (min < glob_min)
            glob_min = min;
        if (max > glob_max)
            glob_max = max;
        std::cout << "Min: " << min << " Max: " << max << std::endl;
    }
;       std::cout << "Global Min: " << glob_min << " Global Max: " << glob_max << std::endl;

    //while (test_evaluator.evaluate_next_img())
    //{
    //    for (int i = 0; i < thresholds.size(); i++)
    //    {
    //        total_false_negatives[i] += test_evaluator.false_negative_range[i];
    //        total_false_positives[i] += test_evaluator.false_positive_range[i];
    //        total_true_positives[i] += test_evaluator.true_positive_range[i];
    //        std::cout << "False negative: " << test_evaluator.false_negative_range[i] << " False Positive: "
    //            << test_evaluator.false_positive_range[i] << " True Positive: " << test_evaluator.true_positive_range[i] << std::endl;
    //    }
    //}

    //int spacing = 17;
    //std::cout << setw(5) << "IoU" << setw(spacing) << "Precision" << setw(spacing) << "Recall" << setw(spacing) << "F1" << setw(spacing)
    //    << "False Negatives" << setw(spacing) << "False Positives" << setw(spacing) << "True Posiives" << std::endl;
    //double avg_precision = 0, avg_recall = 0, avg_f1 = 0;
    //for (int i = 0; i < thresholds.size(); i++)
    //{
    //    double precision = total_true_positives[i] / (total_true_positives[i] + total_false_positives[i]);
    //    double recall = total_true_positives[i] / (total_true_positives[i] + total_false_negatives[i]);
    //    double f1 = (2 * recall * precision) / (recall + precision);
    //    std::cout << setw(5) << thresholds[i] << setw(spacing) << precision << setw(spacing) << recall << setw(spacing) << f1 << setw(spacing)
    //        << total_false_negatives[i] << setw(spacing) << total_false_positives[i] << setw(spacing) << total_true_positives[i] << std::endl;
    //    avg_precision += precision / thresholds.size();
    //    avg_recall += recall / thresholds.size();
    //    avg_f1 += f1 / thresholds.size();
    //}
    //std::cout << setw(5) << "Avg" << setw(spacing) << avg_precision << setw(spacing) << avg_recall << setw(spacing) << avg_f1 << setw(spacing)
    //    << "--" << setw(spacing) << "--" << setw(spacing) << "--" << std::endl;
    ////test_evaluator.print_metrics();
    return 0;
}

// What we want 
// for one threshold value, calculate the average f1 score for different thresholds. 
    // to save time, calculate the whole array of f1 at once.