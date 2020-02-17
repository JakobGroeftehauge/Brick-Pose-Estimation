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

void Brick_Detector::predict_all_images()
{

    while(data_loader.Load_Next() == true)
    {
        std::vector<std::vector<double>> lines = get_lines(data_loader.img);
        boundingBox_detector.detect_BB(lines);
        cv::Mat img_copy = data_loader.img.clone();
        show_predictions(img_copy, boundingBox_detector.BB);
        std::cout << "Size: " << boundingBox_detector.BB.size() << std::endl;
        show_lines(img_copy, lines);
        cv::imshow("Pred", img_copy);
        cv::waitKey(0);
    }

}
