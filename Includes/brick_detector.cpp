#include "brick_detector.h"
#include "Hough_space.h"
#include "data_loader.h"

Brick_Detector::Brick_Detector(std::string path_to_folder)
{
    this->data_loader = Data_loader(path_to_folder);
}

std::vector<std::vector<double> > Brick_Detector::get_lines(cv::Mat img)
{
    cv::Mat gray_img;
    cv::cvtColor(img, gray_img, CV_BGR2GRAY);

    cv::Mat filter_img;
    cv::medianBlur(gray_img, filter_img, 5);

    cv::Mat edge_img;
    cv::Canny(filter_img, edge_img, this->lower_thres_canny, this->uppper_thres_canny);

    Hough_space hough(edge_img);

    return hough.find_lines();
}

void Brick_Detector::show_predictions(cv::Mat img, std::vector<cv::Rect> BB)
{
    for(unsigned int i = 0; i < BB.size(); i++)
    {
        cv::rectangle(img, BB[i].tl(), BB[i].br(), cv::Scalar(255, 0, 0, 1));
    }
}

void Brick_Detector::show_lines(cv::Mat img, vector<vector<double> > lines)
{
    for(unsigned int i =0; i < lines.size(); i++)
    {
        cv::Point pt1, pt2;
        double a = cos(lines[i][1]), b = sin(lines[i][1]);
        double x0 = a * lines[i][0], y0 = b * lines[i][0];
        int size = 1000;//max(img.size().height, img.size().width) * 2; //just needs to be big enough.
        pt1.x=cvRound(x0 + size*(-b)) + img.size().width/2;
        pt1.y=cvRound(y0 + size*(a)) + img.size().height/2;
        pt2.x=cvRound(x0 - size*(-b)) + img.size().width/2;
        pt2.y=cvRound(y0 - size*(a)) + img.size().height/2;
        cv::line(img, pt1, pt2, cv::Scalar(0,255,0), 1, cv::LINE_AA);
    }
}

void Brick_Detector::evaluate_predictions(double threhold = 0.5)
{
    std::vector<cv::Rect> annotations(this->data_loader.Bounding_boxes);
    std::vector<cv::Rect> predictions(this->boundingBox_detector.BB);

    unsigned int i = 0;
    while(i < predictions.size())
    {
        //Find the BB with the greatest intersection over union.
        double max_IOU = 0;
        int index_max_IOU = 0;
        for(unsigned int j = 0; j < annotations.size(); j++)
        {
            double IOU = intersection_area(predictions[i], annotations[j]) / union_area(predictions[i], annotations[j]);
            if(IOU > max_IOU)
            {
                max_IOU = IOU;
                index_max_IOU = j;
            }
        }

        if(max_IOU > threhold)
        {
            //Remove elements form annotation list and prediction list.
            predictions.erase(predictions.begin() + i);
            annotations.erase(annotations.begin() + index_max_IOU);
        }
        else
        {
            i++;
        }
    }
    int true_pos = this->boundingBox_detector.BB.size() - predictions.size();
    int false_pos = predictions.size();
    int false_neg = annotations.size();
    save_evaluation(true_pos, false_pos, false_neg);
}

void Brick_Detector::save_evaluation(int true_pos, int false_pos, int false_neg)
{
    file << data_loader.annotation_loader.imagePath << ", ";
    file << true_pos << ", ";
    file << false_pos << ", ";
    file << false_neg << ", ";
    file << "\n";

}

double Brick_Detector::intersection_area(cv::Rect rect1, cv::Rect rect2)
{
   return (rect1 & rect2).area();
}

double Brick_Detector::union_area(cv::Rect rect1, cv::Rect rect2)
{
    return (rect1 | rect2).area();
}

void Brick_Detector::predict_all_images()
{

    file.open("result.csv");
    for(int i = 0; i < 2; i++)
    //while(data_loader.Load_Next() == true)
    {
        data_loader.Load_Next();
        std::vector<std::vector<double>> lines = get_lines(data_loader.img);
        boundingBox_detector.detect_BB(lines);
        evaluate_predictions(0.5);
        cv::Mat img_copy = data_loader.img.clone();
        show_predictions(img_copy, boundingBox_detector.BB);
        std::cout << "Size: " << boundingBox_detector.BB.size() << std::endl;
        show_lines(img_copy, lines);
        cv::imshow("Pred", img_copy);
        cv::waitKey(0);
    }

    file.close();
}
