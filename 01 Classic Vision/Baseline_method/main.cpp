#include <iostream>
#include "../Includes/Brick_Detector.h"
#include "../Includes/Evaluator.h"
//#include "../Includes/util.h"
//#include "Brick_Detector.h"


using namespace std;

int main()
{
    Evaluator test_evaluator("../Strojer_Images/Only Aligned Dataset");
    //test_evaluator.open_file();
    //test_evaluator.close_file();
    while (test_evaluator.evaluate_next_img(0.5))
    {
        cv::imshow("Image", test_evaluator.img_to_print);
        cv::waitKey(1);
    }
    return 0;
}
