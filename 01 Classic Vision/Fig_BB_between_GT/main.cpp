#include <iostream>
#include <opencv2/opencv.hpp>
#include "../Includes/util.h"

using namespace std;

std::string image_str = "colorIMG_70";



std::vector<cv::Rect> convert_to_rect(std::vector<std::vector<double>> preds, int x_shift, int y_shift)
{
    std::vector<cv::Rect> output;

    for(int i = 0; i < preds.size(); i++)
    {
        output.push_back(cv::Rect(cv::Point2f(preds[i][0] - x_shift, preds[i][1]- y_shift), cv::Point2f(preds[i][2] - x_shift, preds[i][3]- y_shift)));
    }
    return output;
}

int main()
{

    std::vector<std::vector<double>> BB_preds = {{444.58536, 182.85445, 549.83575, 231.30385},
                                                 {439.524 , 291.79224, 544.2153 , 330.54544},
                                                 {441.35748, 215.16922, 545.4128 , 255.61458},
                                                 {438.9872 , 264.69214, 546.8136 , 307.15866},
                                                 {438.8369 , 317.7118 , 546.16486, 357.36438},
                                                 {342.41812, 150.74985, 446.10077, 185.25732},
                                                 {343.94406, 179.8474 , 446.19427, 212.79593},
                                                 {337.9504 , 290.60098, 439.63986, 322.66125},
                                                 {338.74393, 315.69995, 441.92194, 347.99582},
                                                 {446.45358, 151.62517, 553.26166, 206.89998},
                                                 {436.40933, 240.35266, 539.81616, 279.22168},
                                                 {340.6131 , 206.76103, 445.0098 , 239.46117},
                                                 {338.55066, 260.45023, 441.8562 , 290.81055},
                                                 {440.74417, 344.917 , 547.85815, 381.4867 },
                                                 {339.93634, 343.7037 , 443.00406, 375.39413},
                                                 {336.08865, 235.48868, 438.57547, 265.18112},
                                                 {343.06216, 166.40588, 445.69513, 197.08975}};

   int x_shift = 240;
   int y_shift = 100;
   std::vector<cv::Rect> BB_preds_rect = convert_to_rect(BB_preds, x_shift, y_shift);
   cv::Mat img = cv::imread("../../03 Data/Simple Dataset Copied/" + image_str + ".png");
   cv::Rect roi(cv::Point(x_shift, y_shift), cv::Point(x_shift + 385, y_shift + 350));
   img = img(roi);

   for(int i = 0; i < BB_preds_rect.size(); i++)
   {
       //cv::Mat img_clone = img.clone();
       if(i == 16)
       {
           cv::rectangle(img, BB_preds_rect[i].tl(), BB_preds_rect[i].br(), cv::Scalar(0, 0, 255),2);

       }
       else
       {
            cv::rectangle(img, BB_preds_rect[i].tl(), BB_preds_rect[i].br(), cv::Scalar(0, 255, 0),1);
       }


       cv::imshow("img", img);
       std::cout << "Annotation Number: "<<i << std::endl;
       //cv::waitKey(0);

   }

    imwrite("BB_between_TP.png", img);
    cv::waitKey(0);
    cout << "Hello World!" << endl;
    return 0;
}
