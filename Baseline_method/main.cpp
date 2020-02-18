#include <iostream>
#include "../Includes/brick_detector.h"

using namespace std;

int main()
{
    Brick_Detector detector("../Strojer_Images/Initial Test Images/Cropped");
    detector.predict_all_images();
    cout << "Hello World!" << endl;
    return 0;
}
