#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

int main()
{
    cv::Mat image;
    image = cv::imread("../../03 Data/Simple Dataset/colorIMG_6.png");

    //cv::resize(image, image, cv::Size(image.size().width * 1.05, image.size().height * 1.05), 0, 0);

    Chamfer_brick_detector chamfer_detector(image);
    
    std::cout << chamfer_detector.pred_candidates.size() << std::endl;
    std::vector<prediction_candidate> candidates;
    //candidates.insert(candidates.end(), chamfer_detector.pred_candidates.begin(), chamfer_detector.pred_candidates.begin()+35);
    util::print_rotated_bounding_boxes(image, chamfer_detector.pred_candidates);

    std::cout << "angle" << chamfer_detector.predictions[6].angle_vector << " score: " << chamfer_detector.pred_candidates[6].distance_score << std::endl;

    imshow("image", image);


    //chamfer_detector.create_matchingspace(1, 30);
    cv::waitKey(0);

    return 0;
}

