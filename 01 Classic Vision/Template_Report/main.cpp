#include <iostream>
#include "../Includes/Chamfer_brick_detector.h"

using namespace std;



int main()
{
    cv::Mat image;
    image = cv::imread("../../03 Data/Simple Dataset/colorIMG_12.png");
    Chamfer_brick_detector detector(image);
    cv::Mat _template;
    cv::RotatedRect rot_rect;

    detector.create_template(110, 36*4, _template, rot_rect);
    cv::normalize(_template, _template);
    _template *= 65536;
    cv::imshow("Template", _template);
    cv::imshow("Test", image);
    imwrite("Template_1010_144.png", _template);

    cout << "Hello World!" << endl;
    cv::waitKey(0);
    return 0;
}
