#include <iostream>
#include "../Includes_restructured/Brick_Detector.h"
//#include "Brick_Detector.h"

using namespace std;

int main()
{
    cv::Mat img;
    Brick_Detector test;
    test.set_img(img);
    //Brick_Detector detector("../Strojer_Images/Initial Test Images/Cropped");
    //detector.predict_all_images();
    cout << "Hello World!" << endl;
    return 0;
}
