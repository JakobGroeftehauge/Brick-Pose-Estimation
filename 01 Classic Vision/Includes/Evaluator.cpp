#include "Evaluator.h"
#include <experimental/filesystem>


Evaluator::Evaluator()
{
}

Evaluator::Evaluator(std::string path)
{
    if (!(std::experimental::filesystem::exists(path + "/evaluations")))
    {
        if(std::experimental::filesystem::create_directory(path + "/evaluations"))
            std::cout << "Directory: " + path + "/evaluations created" <<  std::endl;
    }
        

	set_path(path);
}

Evaluator::Evaluator(std::string path_to_folder, std::string img_list_csv)
{
    if (!(std::experimental::filesystem::exists(path_to_folder + "/evaluations")))
    {
        if (std::experimental::filesystem::create_directory(path_to_folder + "/evaluations"))
            std::cout << "Directory: " + path_to_folder + "/evaluations created" << std::endl;
    }

    this->loader = Data_loader(path_to_folder, img_list_csv);
}

void Evaluator::set_detector(Detector* detector_addr)
{
    this->detector = detector_addr;
}

void Evaluator::set_path(std::string path)
{

	this->loader = Data_loader(path);
}

bool Evaluator::evaluate_next_img()
{
    if (file.is_open() == false)
    {
        open_file();
    }
	if (loader.loadNext())
	{
		this->detector->detect(loader.img); 
        evaluate_range(this->evaluate_thresholds);
		return true;
	}
	else
	{
		std::cout << "No more images to evaluate" << std::endl;
        close_file();
		return false;
	}
}

void Evaluator::evaluate_dataset(std::vector<double> thresholds)
{
    set_thresholds(thresholds);
    //reset_counters();
    this->results.clear();
    for (int i = 0; i < thresholds.size(); i++) // create an element for each threshold.
    {
        this->results.push_back(evaluation_results(thresholds[i]));
    }
    while (evaluate_next_img()) {}
    for (int i = 0; i < this->results.size(); i++)
    {
        this->results[i].precision = double(this->results[i].total_TP) / double(this->results[i].total_TP + this->results[i].total_FP);
        this->results[i].recall = double(this->results[i].total_TP) / double(this->results[i].total_TP + this->results[i].total_FN);
        this->results[i].f1 = 2 * this->results[i].precision * this->results[i].recall / (this->results[i].precision + this->results[i].recall);
        this->results[i].avg_angle_err = this->results[i].angle_err_sum / double(this->results[i].total_TP);
        this->results[i].std_angle_err = sqrt(this->results[i].angle_err_sqr_sum / double(this->results[i].total_TP) - pow(this->results[i].avg_angle_err, 2));
    }

}

void Evaluator::set_thresholds(std::vector<double> thresholds)
{
    this->evaluate_thresholds = thresholds;
}

void Evaluator::open_file()
{
    //file.open(loader.path_folder + "/evaluation_images/results.csv");
    this->file.open(this->loader.path_folder + "/evaluations/results.csv");
    this->file << "File Name, True Positive, False Positive, False Negative \n";
}

void Evaluator::close_file()
{
    file.close();
}

//void Evaluator::reset_counters()
//{
//    this->total_false_negative = 0.0;
//    this->total_false_positive = 0.0;
//    this->total_true_positive = 0.0;
//}

void Evaluator::print_metrics()
{
    //this function ahs to be changed or deleted
    //std::cout << "Threshold: " << this->evaluate_threshold << std::endl;
    std::cout << "Total False Negatives: " << this->total_false_negative << " Total False Positives: " << this->total_false_positive << "Total True positives: " << this->total_true_positive << std::endl;
    float precision = this->total_true_positive / (this->total_true_positive + this->total_false_positive);
    float recall = this->total_true_positive / (this->total_true_positive + this->total_false_negative);
    float f1 = (2 * recall * precision) / (recall + precision);
    std::cout << "Precision: " << precision << std::endl;
    std::cout << "Recall: " << recall << std::endl;
    std::cout << "F1: " << f1 << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
}

evaluation_results Evaluator::get_avg_result()
{
    evaluation_results output_res(-1);

    for(int i = 0; i < this->results.size(); i++)
    {
        output_res.total_FN += this->results[i].total_FN;
        output_res.total_FP += this->results[i].total_FP;
        output_res.total_TP += this->results[i].total_TP;
        output_res.angle_err_sqr_sum += this->results[i].angle_err_sqr_sum;
        output_res.angle_err_sum += this->results[i].angle_err_sum;
    }
    output_res.precision = double(output_res.total_TP) / double(output_res.total_TP + output_res.total_FP);
    output_res.recall = double(output_res.total_TP) / double(output_res.total_TP + output_res.total_FN);
    output_res.f1 = 2 * output_res.recall * output_res.precision / (output_res.recall + output_res.precision);
    output_res.avg_angle_err = output_res.angle_err_sum / double(output_res.total_TP);
    output_res.std_angle_err = sqrt(output_res.angle_err_sqr_sum / double(output_res.total_TP) - pow(output_res.avg_angle_err, 2.0));


    output_res.total_TP = -1;
    output_res.total_FP = -1;
    output_res.total_FN = -1;
    output_res.angle_err_sum = -1;
    output_res.angle_err_sqr_sum = -1;
    return output_res;
}

double Evaluator::calculate_IoU(cv::Rect rect1, cv::Rect rect2)
{
	return (double)(rect1 & rect2).area() / (double)(rect1 | rect2).area();
}

double Evaluator::calculate_IoU(cv::RotatedRect rect1, cv::RotatedRect rect2)
{
    double intersection_area = 0.0;
    std::vector<cv::Point2f> intersection_cont;
    if (cv::INTERSECT_NONE != cv::rotatedRectangleIntersection(rect1, rect2, intersection_cont))
    {
        std::vector<cv::Point2f> hull;
        cv::convexHull(intersection_cont, hull);
        intersection_area = cv::contourArea(hull);
    }
    else
    {
        intersection_area = 0.0;
    }
    double union_area = rect1.size.area() + rect2.size.area() - intersection_area;
    return intersection_area / union_area;
}

//Evaluates detections in one image at one threshold
//void Evaluator::evaluate(double threshold, int* false_neg_out, int* false_pos_out, int* true_pos_out)
//{
//    this->false_positive = {};
//    this->true_positive = {};
//    //std::cout << "Threshold: " << threshold << std::endl;
//	std::vector<cv::Rect> annotations(this->loader.Bounding_boxes);
//	std::vector<cv::Rect> predictions;
//    for (unsigned int i = 0; i < detector->predictions.size(); i++)
//	{
//		predictions.push_back(detector->predictions[i].rect);
//	}
//    unsigned int i = 0;
//    while (i < predictions.size())
//    {
//        //Find the BB with the greatest intersection over union.
//        double max_IOU = 0;
//        int index_max_IOU = 0;
//        for (unsigned int j = 0; j < annotations.size(); j++)
//        {
//            double IOU = calculate_IoU(predictions[i], annotations[j]);
//            //std::cout << IOU << std::endl;
//            if (IOU > max_IOU)
//            {
//                max_IOU = IOU;
//                index_max_IOU = j;
//            }
//        }
//        if (max_IOU > threshold)
//        {
//            //Remove elements form annotation list and prediction list.
//            this->true_positive.push_back(predictions[i]);
//            predictions.erase(predictions.begin() + i);
//            annotations.erase(annotations.begin() + index_max_IOU);
//        }
//        else
//        {
//            i++;
//        }
//    }
//    this->false_positive = predictions;
//    *true_pos_out = this->detector->predictions.size() - predictions.size();
//    *false_pos_out = predictions.size();
//    //std::cout << "internal true pos:" << this->detector->predictions.size() - predictions.size() << std::endl;
//    *false_neg_out = annotations.size();
//    //save_evaluation(*true_pos_out, *false_pos_out, *false_neg_out);
//    //this->total_false_negative += false_neg;
//    //this->total_false_positive += false_pos;
//    //this->total_true_positive += true_pos;
//}

void Evaluator::evaluate_bb(double threshold, int list[3])
{
    int true_pos = 0;
    int i = 0;
    while ((i < annotation_matches.size())&&annotation_matches[i].IoU > threshold)
    {
        i++;
        true_pos++;
    }
    int false_pos = this->detector->predictions.size() - true_pos;
    int false_neg = this->loader.annotations.size() - true_pos;
    //std::cout << "tru_pos: " << true_pos << " false pos: " << false_pos << " false neg: " << false_neg << std::endl;
    list[0] = true_pos;
    list[1] = false_pos;
    list[2] = false_neg;
}

void Evaluator::evaluate_angle(double threshold, double list[2])
{
    int i = 0;
    double diff = 0;
    double angle_err = 0;
    double angle_err_sqr = 0;
    /*std::ofstream ang_err_file;
    ang_err_file.open(this->loader.path_folder + "/evaluations/" + std::to_string(threshold) + ".csv", std::ofstream::app);*/
    while (i<annotation_matches.size() && annotation_matches[i].IoU > threshold)
    {
        int anno_idx = annotation_matches[i].annotation_idx;
        int pred_idx = annotation_matches[i].prediction_idx;
        diff = this->loader.annotations[anno_idx].angle - this->detector->predictions[pred_idx].angle;
        diff = std::min(abs(diff), std::min(abs(diff + 180), abs(diff - 180)));
        //ang_err_file << loader.annotations[anno_idx].angle << ", " << this->detector->predictions[pred_idx].angle << ", " << diff << "\n";
        angle_err += diff;
        angle_err_sqr += diff * diff;
        i++;
    }
    //ang_err_file.close();
    list[0] = angle_err;
    list[1] = angle_err_sqr;
}

void Evaluator::evaluate_range(std::vector<double> thresholds)
// evaluates one image over a range of IoU thresholds
{
    match_annotations(0); // 0 - use axis aligned
    for (int i = 0; i < thresholds.size(); i++)
    {
        int bbox_res[3];
        double angle_res[2];
        evaluate_bb(thresholds[i], bbox_res);
        evaluate_angle(thresholds[i], angle_res);
        this->results[i].total_FN += bbox_res[2];
        this->results[i].total_FP += bbox_res[1];
        this->results[i].total_TP += bbox_res[0];
        this->results[i].angle_err_sum += angle_res[0];
        this->results[i].angle_err_sqr_sum += angle_res[1];
    }
}



void Evaluator::save_evaluation(int true_pos, int false_pos, int false_neg)
{
    file << loader.file_name << ", ";
    file << true_pos << ", ";
    file << false_pos << ", ";
    file << false_neg << ", ";
    file << "\n";
    // color code true_pos, false_pos etc:
    this->img_to_print = loader.img.clone();
    util::print_bounding_boxes(img_to_print, loader.annotations, cv::Scalar(0, 255, 0));
    //util::print_bounding_boxes(img_to_print, detector.predictions, cv::Scalar(255, 255, 0));
    util::print_bounding_boxes(this->img_to_print, this->false_positive, cv::Scalar(0,0,255));
    util::print_bounding_boxes(this->img_to_print, this->true_positive, cv::Scalar(255, 0, 0));
    //util::print_lines(this->img_to_print, this->detector->lines, cv::Scalar(255, 255, 255));

    cv::imwrite(loader.path_folder + "/evaluations/" + loader.file_name, img_to_print);
}

void Evaluator::match_annotations(int rect_type)
// Calling the function with 0 uses axis aligned bounding boxes, otherwise rotated are used.
{
    annotation_matches.clear();
    if (detector->predictions.empty() || loader.annotations.empty())
    {
        return;
    }
    //std::cout << "prediction length: " << detector->predictions.size() << "annotations length: " << loader.annotations.size() << std::endl;
    cv::Mat IoU_mat = cv::Mat::zeros(detector->predictions.size(), loader.annotations.size(), CV_64F);
    for (int i = 0; i < detector->predictions.size(); i++)
    {
        for (int j = 0; j < loader.annotations.size(); j++)
        {
            if (rect_type == 0) // axis aligned boundingbox
            {
                IoU_mat.at<double>(i,j) = calculate_IoU(detector->predictions[i].rect, loader.annotations[j].rect);
            }
            else
            {
                IoU_mat.at<double>(i, j) = calculate_IoU(detector->predictions[i].rotated_rect, loader.annotations[j].rotated_rect);
            }      
        }
    }
    double max;
    int max_idx[2];
    //do
    //{

    //    cv::minMaxIdx(IoU_mat, NULL, &max, NULL, max_idx);
    //    //std::cout << "max: " << max << " max idx 1:" << max_idx[0] << " idx 2: " << max_idx[1] << std::endl;
    //    IoU_mat.row(max_idx[0]) = 0;
    //    IoU_mat.col(max_idx[1]) = 0;
    //    this->annotation_matches.push_back(annotation_match(max_idx[0], max_idx[1], max));
    //} while (max > 0);

    cv::minMaxIdx(IoU_mat, NULL, &max, NULL, max_idx);
    //std::cout << "max: " << max << " max idx 1:" << max_idx[0] << " idx 2: " << max_idx[1] << std::endl;
    while (max > 0)
    {
        IoU_mat.row(max_idx[0]) = 0;
        IoU_mat.col(max_idx[1]) = 0;
        this->annotation_matches.push_back(annotation_match(max_idx[0], max_idx[1], max));
        cv::minMaxIdx(IoU_mat, NULL, &max, NULL, max_idx);
        //std::cout << "max: " << max << " max idx 1:" << max_idx[0] << " idx 2: " << max_idx[1] << std::endl;
    }
}

