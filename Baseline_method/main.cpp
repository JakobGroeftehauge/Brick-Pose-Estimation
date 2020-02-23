#include <iostream>
#include "../Includes/Brick_Detector.h"
#include "../Includes/Evaluator.h"
//#include "../Includes/util.h"
//#include "Brick_Detector.h"


using namespace std;

int main()
{
    Evaluator test_evaluator("../Strojer_Images/Simple Dataset");
    //test_evaluator.close_file();
    while (test_evaluator.evaluate_next_img())
    {
        cv::imshow("Image", test_evaluator.img_to_print);
        cv::waitKey(0);
    }
    return 0;
}
