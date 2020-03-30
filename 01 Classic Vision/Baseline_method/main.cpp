#include <iostream>
#include "../Includes/Evaluator.h"
//#include "../Includes/util.h"
//#include "Brick_Detector.h"


using namespace std;

int main()
{
    //Brick_Detector detector;
    Chamfer_brick_detector detector;
    Evaluator test_evaluator("../../03 Data/Simple Dataset", "../../02 Deep Learning/Create-CSV-dataset/list_of_img_in_val_set_03-03.csv");
    test_evaluator.set_detector(&detector);
    //test_evaluator.open_file();
    //test_evaluator.close_file();
    float IOU_pct[] = { 0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95 };

    while (test_evaluator.evaluate_next_img(0.5))
    {
        cv::imshow("Image", test_evaluator.img_to_print);
        cv::waitKey(0);

    }

    return 0;
}
