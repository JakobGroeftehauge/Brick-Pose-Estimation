#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

int main()
{
    cv::Mat image;
    image = cv::imread("../../03 Data/Simple Dataset/colorIMG_70.png");

    //cv::resize(image, image, cv::Size(image.size().width * 1.2, image.size().height * 1.2), 0, 0);

    Chamfer_brick_detector chamfer_detector(image);
    
    std::cout << chamfer_detector.pred_candidates.size() << std::endl;
    util::print_rotated_bounding_boxes(image, chamfer_detector.pred_candidates);

    imshow("image", image);


    //chamfer_detector.create_matchingspace(1, 30);
    cv::waitKey(0);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
