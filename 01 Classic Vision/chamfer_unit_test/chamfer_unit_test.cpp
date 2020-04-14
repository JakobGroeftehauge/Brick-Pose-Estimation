#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

int main()
{
    cv::Mat image;
    cv::Mat image_org;
    image_org = cv::imread("../../03 Data/Dataset2_onPallet/OP_colorIMG_269.png");
    image = image_org.clone();
    //image = cv::imread("../../03 Data/Simple Dataset/colorIMG_193.png");
    //1280x720 -> 640x480
    double scale = 2.0/3.0;
    cv::resize(image, image, cv::Size(), scale, scale, CV_INTER_AREA);
    cv::Rect ROI(0, 0, 960*scale, 720*scale);
    cv::Point ROI_shift((image.size().width - ROI.width) / 2, 0);
    ROI = ROI + ROI_shift;

    image = image(ROI);

    Chamfer_brick_detector chamfer_detector(image);
    chamfer_detector.set_NMS_thresh(120);
    chamfer_detector.detect(image);

    std::cout << chamfer_detector.pred_candidates.size() << std::endl;
    std::vector<prediction_candidate> candidates;
    //candidates.insert(candidates.end(), chamfer_detector.pred_candidates.begin(), chamfer_detector.pred_candidates.begin()+35);
    
    for (int i = 0; i < chamfer_detector.pred_candidates.size(); i++)
    {
        chamfer_detector.pred_candidates[i].rotated_rect.center += cv::Point2f(ROI_shift);
        chamfer_detector.pred_candidates[i].rotated_rect.center *= 1.0/scale;
        chamfer_detector.pred_candidates[i].rotated_rect.size.width *= 1.0 / scale;
        chamfer_detector.pred_candidates[i].rotated_rect.size.height *= 1.0 / scale;

    }
    util::print_rotated_bounding_boxes(image_org, chamfer_detector.pred_candidates);

    //std::cout << "angle" << chamfer_detector.predictions[6].angle_vector << " score: " << chamfer_detector.pred_candidates[6].distance_score << std::endl;

    imshow("image", image_org);
    //cv::waitKey(0);

    /*cv::Mat white_template = cv::Mat::ones(chamfer_detector.chamfer_img.rows, chamfer_detector.chamfer_img.cols, chamfer_detector.matching_space_disp.type());
    cv::Mat matRoi = white_template(cv::Rect(0,0,chamfer_detector.model_template.cols,chamfer_detector.model_template.rows));
    cv::Mat conv_template;
    chamfer_detector.model_template.convertTo(conv_template, chamfer_detector.matching_space_disp.type());
    conv_template.copyTo(matRoi);
    white_template *= 255;
    matRoi = white_template(cv::Rect(chamfer_detector.model_template.cols-1,chamfer_detector.model_template.rows-1,chamfer_detector.matching_space_disp.cols, chamfer_detector.matching_space_disp.rows));
    chamfer_detector.matching_space_disp.copyTo(matRoi);


    cv::imshow("mat roi", conv_template);
    cv::imshow("Template", white_template);
    cv::imshow("distance map", chamfer_detector.chamfer_img);
    imwrite("Distance_transform.png", chamfer_detector.chamfer_img);
    imwrite("Matching_space.png", white_template);

    cv::imshow("chamfer image", chamfer_detector.chamfer_img);*/
    //chamfer_detector.create_matchingspace(1, 30);
    cv::waitKey(0);

    return 0;
}
