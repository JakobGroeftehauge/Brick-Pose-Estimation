#include <iostream>
#include <opencv2/opencv.hpp>
#include "../Includes/annotation_loader.h"


using namespace std;

// OP dataset
string path = "../../03 Data/Dataset2_onPallet/" ;
string file_name =  "OP_colorIMG_185";
cv::Rect roi(420, 80, 800, 600);
int line_thickness = 3;

// Simple
/*string path = "../../03 Data/Simple Dataset Copied/" ;
string file_name =  "colorIMG_159";
cv::Rect roi(230, 100, 400, 320);
int line_thickness = 2;*/

std::vector<cv::RotatedRect> convert_to_rot_rect(std::vector<std::vector<float>> rot_rect)
{
    std::vector<cv::RotatedRect> output;
    for(int i = 0; i < rot_rect.size();i++)
    {
        output.push_back(cv::RotatedRect(cv::Point2f(rot_rect[i][0], rot_rect[i][1]), cv::Size2f(rot_rect[i][2], rot_rect[i][3]), rot_rect[i][4]));
    }

    return output;
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

void draw_rot_rect(cv::Mat& img, cv::RotatedRect rot_rect, cv::Scalar color, int line_width)
{
    cv::Point2f rect_points[4];
    rot_rect.points(rect_points);

    for (int j = 0; j < 4; j++)
    {
        cv::line(img, rect_points[j], rect_points[(j + 1) % 4], color, line_width, CV_AA);
    }
}

void print_angle_predictions(cv::Mat& img, cv::RotatedRect BB, int line_width, cv::Scalar color = cv::Scalar(255, 255, 0))
{
    double w = BB.size.width;
    double h = BB.size.height;
    double angle = BB.angle;
    cv::Point2d pt1 = BB.center + cv::Point2f(w/2.0*sin((angle+90)*CV_PI/180)*0.5, -w/2.0*cos((angle+90)*CV_PI/180)*0.5);
    cv::Point2d pt2 = BB.center -  cv::Point2f(w/2.0*sin((angle+90)*CV_PI/180)*0.5, -w/2.0*cos((angle+90)*CV_PI/180)*0.5);

    cv::arrowedLine(img, pt2, pt1, color, line_width, CV_AA, 0, 0.15);
}

void match_annotations(std::vector<cv::RotatedRect> preds, std::vector<std::vector<cv::Point2f>> GT, std::vector<int> &TP, double IoU_thres)
// Calling the function with 0 uses axis aligned bounding boxes, otherwise rotated are used.
{
    //std::cout << "prediction length: " << detector->predictions.size() << "annotations length: " << loader.annotations.size() << std::endl;
    cv::Mat IoU_mat = cv::Mat::zeros(preds.size(), GT.size(), CV_64F);
    for (int i = 0; i < preds.size(); i++)
    {
        for (int j = 0; j < GT.size(); j++)
        {

                IoU_mat.at<double>(i, j) = calculate_IoU(preds[i], cv::minAreaRect(GT[j]));
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
    cout << "Hello World!" << endl;

    // Simple data set
    //std::vector<std::vector<float>> rot_rect =  {{395.595458984375, 175.3562774658203, 96.34921, 23.470917, -45.91988088437265}, {382.73565673828125, 153.85047912597656, 97.78876, 23.62027, -44.90274960374185}, {312.9042663574219, 244.63446044921875, 98.80499, 23.288513, -57.08363042236714}, {435.47509765625, 331.9866943359375, 99.61536, 23.55722, -56.93260127943982}, {462.3162841796875, 219.9646453857422, 100.1311, 23.795624, -52.72588776962897}, {370.56524658203125, 293.03240966796875, 100.39859, 23.530975, -57.26363322775365}, {352.45257568359375, 274.4505615234375, 99.586945, 24.062042, -58.1844964071277}, {416.06658935546875, 194.32745361328125, 99.54187, 23.600967, -49.50036166994171}, {438.3115234375, 209.90969848632812, 98.63437, 23.20935, -52.298953135178984},
    //                                              {498.21337890625, 253.1175537109375, 97.98718, 23.566528, -55.51137210184898}, {518.0841674804688, 268.385009765625, 98.35791, 23.795288, -53.27306105422785}, {332.2288818359375, 260.9841613769531, 99.978516, 23.071365, -57.56338632476342}, {478.2115478515625, 239.36758422851562, 98.4884, 23.378677, -52.83878055132553}, {455.73321533203125, 346.6509704589844, 100.115234, 24.072754, -57.39241985964883}, {391.512451171875, 306.4643249511719, 100.70032, 23.720215, -57.55556575816375}, {413.77294921875, 317.7061767578125, 100.74063, 23.53949, -57.6405811227629}};

    // OP dataset
    std::vector<std::vector<float>> rot_rect = {{1020.951171875, 437.3903503417969, 181.42108, 43.093567, -40.39099984559824}, {731.5518798828125, 374.6361389160156, 178.96362, 42.960876, -40.2205217390093}, {993.0494384765625, 403.96319580078125, 181.79315, 43.44449, -39.48199387462333}, {877.0338134765625, 261.0292053222656, 184.84424, 43.0802, -35.83536612203728}, {907.031494140625, 296.79302978515625, 180.43353, 41.93521, -38.69199028955756}, {757.4960327148438, 409.3266296386719, 177.78796, 42.31006, -40.295602593460664}, {934.1951293945312, 332.4552001953125, 180.57239, 42.95706, -40.99107294531888}, {878.583984375, 554.6513671875, 182.7768, 44.00586, -41.18454829884324}, {706.7969970703125, 338.9176940917969, 182.91266, 44.212036, -40.318695463044484}, {962.276123046875, 366.99603271484375, 181.55347, 43.457, -40.972819264762016}, {843.7822265625, 518.6263427734375, 183.6275, 44.089783, -42.166138690127866}, {814.7509765625, 477.63128662109375, 183.45294, 43.980286, -41.13272082342118}, {787.3956298828125, 444.9378662109375, 179.20691, 42.68814, -38.97791566859264}};

    std::vector<cv::RotatedRect> predictions = convert_to_rot_rect(rot_rect);

    cv::Mat img = cv::imread(path + file_name + ".png", cv::IMREAD_COLOR);;

    Annotation_loader anno_loader;
    anno_loader.loadAnnotation(path + file_name + ".json");
    std::vector<std::vector<cv::Point2f>> annotations = anno_loader.Rect_list;

    std::vector<int> TP;

    match_annotations(predictions, annotations, TP, 0.5);

    for(int i = 0; i < predictions.size(); i++)
    {
       if(std::find(TP.begin(), TP.end(), i) != TP.end())
       {
           draw_rot_rect(img, predictions[i], cv::Scalar(0, 255, 0), line_thickness); //simple width - 2 OP - 3
           print_angle_predictions(img, predictions[i], line_thickness);
       }
       else
       {
           draw_rot_rect(img, predictions[i], cv::Scalar(0, 0, 255), line_thickness + 1);  //simple width - 3 OP - 4
           //print_angle_predictions(img_axis_non_aligned, preds[i], 2);
       }
    }

    cv::imshow("test", img);

    imwrite(file_name + "_Rotinanet_non_axis_aligned.png", img(roi));
    cv::waitKey(0);

    return 0;
}
