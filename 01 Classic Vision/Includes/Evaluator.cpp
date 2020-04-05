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

void Evaluator::reset_counters()
{
    this->total_false_negative = 0.0;
    this->total_false_positive = 0.0;
    this->total_true_positive = 0.0;
}

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

double Evaluator::calculate_IoU(cv::Rect rect1, cv::Rect rect2)
{
	return (double)(rect1 & rect2).area() / (double)(rect1 | rect2).area();
}

//Evaluates detections in one image at one threshold
void Evaluator::evaluate(double threshold, int* false_neg_out, int* false_pos_out, int* true_pos_out)
{
    this->false_positive = {};
    this->true_positive = {};
    //std::cout << "Threshold: " << threshold << std::endl;
	std::vector<cv::Rect> annotations(this->loader.Bounding_boxes);
	std::vector<cv::Rect> predictions;
    for (unsigned int i = 0; i < detector->predictions.size(); i++)
	{
		predictions.push_back(detector->predictions[i].rect);
	}
    unsigned int i = 0;
    while (i < predictions.size())
    {
        //Find the BB with the greatest intersection over union.
        double max_IOU = 0;
        int index_max_IOU = 0;
        for (unsigned int j = 0; j < annotations.size(); j++)
        {
            double IOU = calculate_IoU(predictions[i], annotations[j]);
            //std::cout << IOU << std::endl;
            if (IOU > max_IOU)
            {
                max_IOU = IOU;
                index_max_IOU = j;
            }
        }

        if (max_IOU > threshold)
        {
            //Remove elements form annotation list and prediction list.
            this->true_positive.push_back(predictions[i]);
            predictions.erase(predictions.begin() + i);
            annotations.erase(annotations.begin() + index_max_IOU);
        }
        else
        {
            i++;
        }
    }

    this->false_positive = predictions;

    *true_pos_out = this->detector->predictions.size() - predictions.size();
    *false_pos_out = predictions.size();
    //std::cout << "internal true pos:" << this->detector->predictions.size() - predictions.size() << std::endl;
    *false_neg_out = annotations.size();
    save_evaluation(*true_pos_out, *false_pos_out, *false_neg_out);
    //this->total_false_negative += false_neg;
    //this->total_false_positive += false_pos;
    //this->total_true_positive += true_pos;
}

void Evaluator::evaluate_range(std::vector<double> thresholds)
{
    this->false_negative_range.clear();
    this->false_positive_range.clear();
    this->true_positive_range.clear();
    for (int i = 0; i < thresholds.size(); i++)
    {
        int false_neg = 0;
        int false_pos = 0;
        int true_pos = 0;
        evaluate(thresholds[i], &false_neg, &false_pos, &true_pos);
        //std::cout << i << " external true pos: " << true_pos << std::endl;
        this->false_negative_range.push_back(false_neg);
        this->false_positive_range.push_back(false_pos);
        this->true_positive_range.push_back(true_pos);
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
    util::print_bounding_boxes(img_to_print, loader.Bounding_boxes, cv::Scalar(0, 255, 0));
    //util::print_bounding_boxes(img_to_print, detector.predictions, cv::Scalar(255, 255, 0));
    util::print_bounding_boxes(this->img_to_print, this->false_positive, cv::Scalar(0,0,255));
    util::print_bounding_boxes(this->img_to_print, this->true_positive, cv::Scalar(255, 0, 0));
    //util::print_lines(this->img_to_print, this->detector->lines, cv::Scalar(255, 255, 255));

    cv::imwrite(loader.path_folder + "/evaluations/" + loader.file_name, img_to_print);
}
