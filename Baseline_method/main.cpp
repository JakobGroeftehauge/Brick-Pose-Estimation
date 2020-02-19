#include <iostream>
#include "../Includes_restructured/Brick_Detector.h"
#include "../Includes_restructured/Evaluator.h"
//#include "Brick_Detector.h"


using namespace std;

int main()
{
    cv::Mat img = cv::imread("/home/jakob/Documents/Bachelor Project/Strojer_Images/Initial Test Images/Cropped/IMG_4049.JPG", CV_LOAD_IMAGE_COLOR);
    /*cv::imshow("test", img);
    cv::waitKey(0);*/
    Brick_Detector test;
    test.set_img(img);
    test.detect();
    //Brick_Detector detector("../Strojer_Images/Initial Test Images/Cropped");
    //detector.predict_all_images();
    cout << "Hello World!" << endl;
    return 0;
}
