#include <iostream>
#include <string>
#include "../Includes/Brick_Detector.h"
#include "../Includes/Chamfer_brick_detector.h"
#include "../Includes/annotation_loader.h"
#include "../Includes/predictions.h"

using namespace std;

// OP dataset
//string path = "../../03 Data/Dataset2_onPallet/" ;
//string file_name =  "OP_colorIMG_185";

// Simple
string path = "../../03 Data/Simple Dataset Copied/" ;
string file_name =  "colorIMG_159";

void print_angle_predictions(cv::Mat& img, bounding_box BB, int line_width, cv::Scalar color = cv::Scalar(255, 255, 0))
{
    double w = BB.rotated_rect.size.width;
    double h = BB.rotated_rect.size.height;
    double angle = BB.rotated_rect.angle;
    cv::Point2d pt1 = BB.rotated_rect.center + cv::Point2f(w/2.0*sin((angle+90)*CV_PI/180)*0.5, -w/2.0*cos((angle+90)*CV_PI/180)*0.5);
    cv::Point2d pt2 = BB.rotated_rect.center -  cv::Point2f(w/2.0*sin((angle+90)*CV_PI/180)*0.5, -w/2.0*cos((angle+90)*CV_PI/180)*0.5);

    cv::arrowedLine(img, pt2, pt1, color, line_width, CV_AA,0,0.15);
}


void draw_rot_rect(cv::Mat& img, cv::RotatedRect rot_rect, cv::Scalar color, int line_width)
{
    cv::Point2f rect_points[4];
    rot_rect.points(rect_points);

    for (int j = 0; j < 4; j++)
    {
        cv::line(img, rect_points[j], rect_points[(j + 1) % 4], color, line_width, CV_AA);
    }
}



double calculate_IoU(cv::Rect rect1, cv::Rect rect2)
{
    return (double)(rect1 & rect2).area() / (double)(rect1 | rect2).area();
}

double calculate_IoU(cv::RotatedRect rect1, cv::RotatedRect rect2)
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

void match_annotations(std::vector<bounding_box> preds, std::vector<std::vector<cv::Point2f>> GT, std::vector<int> &TP, double IoU_thres, int rect_type)
// Calling the function with 0 uses axis aligned bounding boxes, otherwise rotated are used.
{
    //std::cout << "prediction length: " << detector->predictions.size() << "annotations length: " << loader.annotations.size() << std::endl;
    cv::Mat IoU_mat = cv::Mat::zeros(preds.size(), GT.size(), CV_64F);
    for (int i = 0; i < preds.size(); i++)
    {
        for (int j = 0; j < GT.size(); j++)
        {
            if (rect_type == 0) // axis aligned boundingbox
            {

                IoU_mat.at<double>(i,j) = calculate_IoU(preds[i].rect, cv::boundingRect(GT[j]));
            }
            else
            {
                IoU_mat.at<double>(i, j) = calculate_IoU(preds[i].rotated_rect, cv::minAreaRect(GT[j]));
            }
        }
    }



    double max;
    int max_idx[2];
    cv::minMaxIdx(IoU_mat, NULL, &max, NULL, max_idx);
    while (max > IoU_thres)
    {
        IoU_mat.row(max_idx[0]) = 0;
        IoU_mat.col(max_idx[1]) = 0;
        TP.push_back(max_idx[0]);
        cv::minMaxIdx(IoU_mat, NULL, &max, NULL, max_idx);
    }


}




int main()
{
   Chamfer_brick_detector detector;
   // OP
   //detector.set_canny_thresh(25, 70);
   //detector.set_NMS_thresh(175);
   //detector.set_brick_specs(139, 30, 211, 54.39);

   // Simple
   detector.set_canny_thresh(25, 70);
   detector.set_NMS_thresh(175);
   detector.set_brick_specs(96, 21, 108, 28);

   std::vector<bounding_box> preds;
   std::vector<std::vector<cv::Point2f>> rect_list;

   cv::Mat img;
   img = cv::imread(path + file_name + ".png", cv::IMREAD_COLOR);

   cv::Mat img_axis_aligned = img.clone();
   cv::Mat img_axis_non_aligned = img.clone();;

   Annotation_loader anno_loader;
   anno_loader.loadAnnotation(path + file_name + ".json");
   rect_list = anno_loader.Rect_list;
   std::cout << "Annotation size" << rect_list.size() << std::endl;

   detector.detect(img);
   preds = detector.predictions;

   // Evaluate OP dataset axis aligned bounding boxes
   std::cout << "Prediction size" << preds.size() << std::endl;
   std::vector<int> TP;

   match_annotations(preds, rect_list, TP, 0.5, 0);

   std::cout << "TP size " << TP.size() << std::endl;
   for(int i = 0; i < preds.size(); i++)
   {
       if(std::find(TP.begin(), TP.end(), i) != TP.end())
       {
           cv::rectangle(img_axis_aligned, preds[i].rect.tl(), preds[i].rect.br(), cv::Scalar(0, 255, 0), 2);
       }
       else
       {
           cv::rectangle(img_axis_aligned, preds[i].rect.tl(), preds[i].rect.br(), cv::Scalar(0, 0, 255), 3);
       }

       std::cout << TP[i] << std::endl;
   }

   for(int i = 0; i < preds.size(); i++)
   {
       if(std::find(TP.begin(), TP.end(), i) != TP.end())
       {
           print_angle_predictions(img_axis_aligned, preds[i], 2);
       }
       std::cout << TP[i] << std::endl;
   }

   // Evaluate OP dataset non-axis aligned bounding boxes
   TP.clear();
   match_annotations(preds, rect_list, TP, 0.5, 1);

   std::cout << "TP size " << TP.size() << std::endl;
   for(int i = 0; i < preds.size(); i++)
   {
       if(std::find(TP.begin(), TP.end(), i) != TP.end())
       {
           draw_rot_rect(img_axis_non_aligned, preds[i].rotated_rect, cv::Scalar(0, 255, 0), 2); //simple width - 2 OP - 3
           print_angle_predictions(img_axis_non_aligned, preds[i], 2);
       }
       else
       {
           draw_rot_rect(img_axis_non_aligned, preds[i].rotated_rect, cv::Scalar(0, 0, 255), 3);  //simple width - 3 OP - 4
       }

       std::cout << TP[i] << std::endl;
   }

    // roi OP dataset
    //cv::Rect roi(420, 80, 800, 600);


   // roi simple
   cv::Rect roi(230, 100, 400, 320);

   // roi simple
   img_axis_aligned = img_axis_aligned(roi);
   img_axis_non_aligned = img_axis_non_aligned(roi);

   cv::imshow(file_name + "_chamfer_axis_aligned", img_axis_aligned);
   cv::imshow(file_name + "_chamfer_no_axis_aligned", img_axis_non_aligned);

   imwrite(file_name + "_chamfer_axis_aligned.png", img_axis_aligned);
   imwrite(file_name + "_chamfer_no_axis_aligned.png", img_axis_non_aligned);

   cv::waitKey(0);

   cout << "Hello World!" << endl;
   return 0;
}
