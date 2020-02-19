#pragma once
#include <iostream>
#include "../Includes_restructured/Brick_Detector.h"
#include "../Includes_restructured/Evaluator.h"
//#include "../Includes_restructured/util.h"
//#include "Brick_Detector.h"


using namespace std;

int main()
{
    Evaluator test_evaluator("../Strojer_Images/Initial Test Images/Cropped");
    //test_evaluator.open_file();
    //test_evaluator.close_file();
    while (test_evaluator.evaluate_next_img())
    {
        cv::imshow("Image", test_evaluator.img_to_print);
        cv::waitKey(0);
    }
    return 0;
}
