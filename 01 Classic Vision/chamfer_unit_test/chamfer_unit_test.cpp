#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

int main()
{
    cv::Mat image;
    image = cv::imread("../../03 Data/Simple Dataset/colorIMG_193.png");

    //cv::resize(image, image, cv::Size(image.size().width * 1.05, image.size().height * 1.05), 0, 0);

    Chamfer_brick_detector chamfer_detector(image);
    chamfer_detector.detect(image);
    
    std::cout << chamfer_detector.pred_candidates.size() << std::endl;
    std::vector<prediction_candidate> candidates;
    //candidates.insert(candidates.end(), chamfer_detector.pred_candidates.begin(), chamfer_detector.pred_candidates.begin()+35);
    util::print_rotated_bounding_boxes(image, chamfer_detector.pred_candidates);

    //std::cout << "angle" << chamfer_detector.predictions[6].angle_vector << " score: " << chamfer_detector.pred_candidates[6].distance_score << std::endl;

    imshow("image", image);
    //cv::waitKey(0);

    cv::Mat white_template = cv::Mat::ones(chamfer_detector.chamfer_img.rows, chamfer_detector.chamfer_img.cols, chamfer_detector.matching_space_disp.type());
    cv::Mat matRoi = white_template(cv::Rect(0,0,chamfer_detector.model_template.cols,chamfer_detector.model_template.rows));
    cv::Mat conv_template;
    chamfer_detector.model_template.convertTo(conv_template, chamfer_detector.matching_space_disp.type());
    conv_template.copyTo(matRoi);
    white_template *= 255;
    matRoi = white_template(cv::Rect(chamfer_detector.model_template.cols-1,chamfer_detector.model_template.rows-1,chamfer_detector.matching_space_disp.cols, chamfer_detector.matching_space_disp.rows));
    chamfer_detector.matching_space_disp.copyTo(matRoi);

    //cv::imshow("mat roi", conv_template);
    cv::imshow("Matching space with template", white_template);
    imwrite("Distance_transform.png", chamfer_detector.chamfer_img);
    imwrite("Matching_space.png", white_template);

    cv::imshow("chamfer image", chamfer_detector.chamfer_img);
    //chamfer_detector.create_matchingspace(1, 30);
    cv::waitKey(0);

    return 0;
}

