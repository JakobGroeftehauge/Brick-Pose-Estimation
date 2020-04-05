#include <iostream>
#include  "../Includes/Chamfer_brick_detector.h"

using namespace std;

int main()
{
    cv::Mat image;
    image = cv::imread("../../03 Data/Simple Dataset/colorIMG_12.png");

    //cv::resize(image, image, cv::Size(image.size().width * 1.05, image.size().height * 1.05), 0, 0);

    Chamfer_brick_detector chamfer_detector(image);
    chamfer_detector.detect(image);
    //util::print_bounding_boxes(image, chamfer_detector.predictions);
    util::print_filled_rotated_bounding_boxes(image, chamfer_detector.predictions, 0.3);
    //util::print_rotated_bounding_boxes(image, chamfer_detector.predictions);
    //util::print_bounding_boxes(image, chamfer_detector.predictions, cv::Scalar(255, 0, 0));
    cv::imshow("Image with BB", image);

    cout << "Hello World!" << endl;
    cv::waitKey(0);
    return 0;
}
