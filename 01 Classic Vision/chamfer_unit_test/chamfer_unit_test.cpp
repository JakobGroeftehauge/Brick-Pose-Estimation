#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

int main()
{
    cv::Mat image;
    image = cv::imread("../../03 Data/Simple Dataset/colorIMG_38.png");
    Chamfer_brick_detector chamfer_detector(image);
    cv::Mat chamfer = chamfer_detector.chamfer_img;
    cv::normalize(chamfer, chamfer, 0, 1.0, cv::NORM_MINMAX);
    cv::imshow("test", chamfer);
    cv::imshow("template", chamfer_detector.model_template);

    cv::Mat matching = chamfer_detector.matching_space;
    cv::normalize(matching, matching, 0, 1.0, cv::NORM_MINMAX);
    cv::imshow("matching space", matching);
    std::cout << "Hello World!\n";

    std::cout << "best rect lenght:" << chamfer_detector.best_rects.size() << std::endl;

    util::print_rotated_bounding_boxes(image, chamfer_detector.pred_candidates);
    cv::imshow("image with rectangles", image);

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
