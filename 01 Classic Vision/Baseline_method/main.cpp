#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

using namespace std;

void print_results(int spacing, std::vector<double> thresholds, std::vector<double> total_false_negatives, std::vector<double> total_false_positives,
    std::vector<double> total_true_positives, std::vector<double> total_angle_err, std::vector<double> total_angle_err_sqr,
    double time, std::ofstream& file, std::ofstream& file2)
{
    file2 << setw(5) << "IoU" << setw(spacing) << "Prec." << setw(spacing) << "Rec." << setw(spacing) << "F1" << setw(spacing)
        << "ang. err." << setw(spacing) << "a. e. std." << setw(spacing) << "F. Neg" << setw(spacing) << "F. Pos" << setw(spacing) << "T. Pos" << std::endl;
    double avg_precision = 0, avg_recall = 0, avg_f1 = 0;
    for (int i = 0; i < thresholds.size(); i++)
    {
        double precision = total_true_positives[i] / (total_true_positives[i] + total_false_positives[i]);
        double recall = total_true_positives[i] / (total_true_positives[i] + total_false_negatives[i]);
        double f1 = (2 * recall * precision) / (recall + precision);
        double angle_err_mean = total_angle_err[i] / double(total_true_positives[i]);
        double angle_err_std = sqrt((total_angle_err_sqr[i] / double(total_true_positives[i])) - pow(angle_err_mean, 2));
        file2 << setw(5) << thresholds[i] << setw(spacing) << precision << setw(spacing) << recall << setw(spacing) << f1 << setw(spacing)
           << angle_err_mean << setw(spacing) << angle_err_std << setw(spacing) << total_false_negatives[i] << setw(spacing) << total_false_positives[i] << setw(spacing) << total_true_positives[i] << std::endl;
        avg_precision += precision / thresholds.size();
        avg_recall += recall / thresholds.size();
        avg_f1 += f1 / thresholds.size();
    }
    file2 << setw(5) << "Avg" << setw(spacing) << avg_precision << setw(spacing) << avg_recall << setw(spacing) << avg_f1 << setw(spacing)
        << "--" << setw(spacing) << "--" << setw(spacing) << "--" << setw(spacing) << "--" << setw(spacing) << "--" << std::endl;
    file2 << "Time: " << time << std::endl;
    file << ", " << avg_f1;
}

void eval_NMS_thresh_chamfer_detector(double NMS_thresh, std::ofstream& file1, std::ofstream& file2) {
    int spacing = 17; // spacing for table print at the end
    std::vector<double> thresholds({ 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 });
    std::vector<double> total_false_negatives(thresholds.size(), 0);
    std::vector<double> total_false_positives(thresholds.size(), 0);
    std::vector<double> total_true_positives(thresholds.size(), 0);
    std::vector<double> total_angle_err(thresholds.size(), 0);
    std::vector<double> total_angle_err_sqr(thresholds.size(), 0);
    Chamfer_brick_detector detector;
    Evaluator test_evaluator("../../03 Data/Simple Dataset", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_12-03.csv");
    test_evaluator.set_detector(&detector);
    test_evaluator.set_thresholds(thresholds);
    //test_evaluator.reset_counters();
    test_evaluator.detector->set_NMS_thresh(NMS_thresh);
    while (test_evaluator.evaluate_next_img())
    {
        for (int i = 0; i < thresholds.size(); i++)
        {
            total_false_negatives[i] += test_evaluator.false_negative_range[i];
            total_false_positives[i] += test_evaluator.false_positive_range[i];
            total_true_positives[i] += test_evaluator.true_positive_range[i];
            total_angle_err[i] += test_evaluator.angle_err_range[i];
            total_angle_err_sqr[i] += test_evaluator.angle_err_sqr_range[i];
            //std::cout << "False negative: " << test_evaluator.false_negative_range[i] << " False Positive: "
              //  << test_evaluator.false_positive_range[i] << " True Positive: " << test_evaluator.true_positive_range[i] << std::endl;
        }
    }
    std::cout << "Evaluation at NMS threshold: " << NMS_thresh << std::endl;
    print_results(spacing, thresholds, total_false_negatives, total_false_positives, total_true_positives, total_angle_err,total_angle_err_sqr,test_evaluator.detector->time, file1, file2);

}

void grid_search_hough_canny_thresh(int low_thresh, int high_thresh, std::ofstream& file, std::ofstream& file2)
{
    int spacing = 12; // spacing for table print at the end
    std::vector<double> thresholds({ 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95 });
    std::vector<double> total_false_negatives(thresholds.size(), 0);
    std::vector<double> total_false_positives(thresholds.size(), 0);
    std::vector<double> total_true_positives(thresholds.size(), 0);
    std::vector<double> total_angle_err(thresholds.size(), 0);
    std::vector<double> total_angle_err_sqr(thresholds.size(), 0);
    Brick_Detector detector; // hough based brick detector
    Evaluator test_evaluator("../../03 Data/Simple Dataset Copied", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_03-03.csv");
    test_evaluator.set_detector(&detector);
    test_evaluator.set_thresholds(thresholds);
    //test_evaluator.reset_counters();
    test_evaluator.detector->set_canny_thresh(low_thresh, high_thresh);
    while (test_evaluator.evaluate_next_img())
    {
        for (int i = 0; i < thresholds.size(); i++)
        {
            total_false_negatives[i] += test_evaluator.false_negative_range[i];
            total_false_positives[i] += test_evaluator.false_positive_range[i];
            total_true_positives[i] += test_evaluator.true_positive_range[i];
            total_angle_err[i] += test_evaluator.angle_err_range[i];
            total_angle_err_sqr[i] += test_evaluator.angle_err_sqr_range[i];
        }
    }
    file2 << "Threshold low: " << low_thresh << " Threshold high" << high_thresh << std::endl;
    print_results(spacing, thresholds, total_false_negatives, total_false_positives, total_true_positives, total_angle_err, total_angle_err_sqr, test_evaluator.detector->time, file, file2);
}

int main()
{
    double steps = 20;
    double min = 100;
    double max = 500;//169942;
    double step_size = (max - min) / steps;
    //int canny_low_min = 38;//0;
    //int canny_low_max = 102;//255;
    //int canny_hyst_min = 66;//5;
    //int canny_hyst_max = 152; //250;
    //int canny_low_steps = 6;
    //int canny_hyst_steps = 8;
    //double canny_low_step_size = (double(canny_low_max) - double(canny_low_min)) / double(canny_low_steps);
    //double canny_hyst_step_size = (double(canny_hyst_max) - double(canny_hyst_min)) / double(canny_hyst_steps);
    //for (int i = 1; i <= steps; i++)
    //{
    //    eval_NMS_thresh_chamfer_detector(min + i * step_size);
    //    std::cout << "--------------------------------------------------------------------------------- \n" << std::endl;
    //}
    std::string test_name = "canny_grid_med_masked";
    std::ofstream grid_search_file, dump_file;
    std::vector<int> canny_lows ={ 51, 63, 76, 89 }; // { 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180};//
    std::vector<int> canny_hysts = { 90, 103, 115, 127,}; //{ 15, 30, 45,  60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225};
    grid_search_file.open("../../03 Data/Simple Dataset Copied/"+test_name+"res.csv");
    dump_file.open("../../03 Data/Simple Dataset Copied/"+ test_name +"dump.txt");
    for (int i = 0; i < canny_hysts.size(); i++)
    {
        grid_search_file << ", " << canny_hysts[i] ;
    }

    for (int i = 0; i < canny_lows.size(); i++)
    {
        grid_search_file << "\n";
        for (int j = 0; j < canny_hysts.size(); j++)
        {
            int canny_low = canny_lows[i];
            int canny_high = canny_lows[i] + canny_hysts[j];
            grid_search_file << canny_low;
            grid_search_hough_canny_thresh(canny_low, canny_high, grid_search_file, dump_file);
        }
    }
    grid_search_file.close();
    dump_file.close();
    return 0;
}

// What we want
// for one threshold value, calculate the average f1 score for different thresholds.
    // to save time, calculate the whole array of f1 at once.
