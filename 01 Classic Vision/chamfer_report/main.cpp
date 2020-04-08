#include <iostream>
#include  "../Includes/Chamfer_brick_detector.h"

using namespace std;

int main()
{
    cv::Mat image;
    image = cv::imread("../../03 Data/Simple Dataset/colorIMG_193.png");


    // Prediction
    cv::Mat image_rot_BB = image.clone();
    cv::Mat image_BB = image.clone();
    //cv::resize(image, image, cv::Size(image.size().width * 1.05, image.size().height * 1.05), 0, 0);

    Chamfer_brick_detector chamfer_detector(image);
    chamfer_detector.detect(image);
    //util::print_bounding_boxes(image, chamfer_detector.predictions);
    util::print_filled_rotated_bounding_boxes(image_rot_BB, chamfer_detector.predictions, 0.3, cv::Scalar(255,0,255));
    util::print_rotated_bounding_boxes(image_rot_BB, chamfer_detector.predictions, cv::Scalar(255,0,255));
    util::print_bounding_boxes(image_BB, chamfer_detector.predictions, cv::Scalar(255, 0, 255));
    cv::imshow("Image with rot BB", image_rot_BB);
    cv::imshow("Image with BB", image_BB);

    imwrite("chamfer_pred_rotBB.png", image_rot_BB);
    imwrite("chamfer_pred_BB.png", image_BB);


    // Figure Displaying how NMS works
    chamfer_detector.set_resolution(30, 110, 100, 5);
    chamfer_detector.set_NMS_thresh(200);
    chamfer_detector.detect(image);

    image = cv::imread("../../03 Data/Simple Dataset/colorIMG_193.png");

    int len_best_fit = 0;
    int len_worst_fit = 30;
    std::vector<prediction_candidate> best_fit(20);
    std::vector<prediction_candidate> worst_fit(20);

    std::copy(chamfer_detector.pred_candidates_unfiltered.begin(), chamfer_detector.pred_candidates_unfiltered.begin() + len_best_fit, best_fit.begin());
    std::copy(chamfer_detector.pred_candidates_unfiltered.end() - len_worst_fit, chamfer_detector.pred_candidates_unfiltered.end(), worst_fit.begin());
    util::print_rotated_bounding_boxes(image, best_fit, cv::Scalar(255, 0, 255), false);
    util::print_rotated_bounding_boxes(image, worst_fit, cv::Scalar(255, 0, 255), false);
    util::print_rotated_bounding_boxes(image, chamfer_detector.predictions, cv::Scalar(255, 0, 255));


    cv::imshow("Image", image);
    imwrite("prediction_not_suppressed.png", image);

    cout << "Hello World!" << endl;
    cv::waitKey(0);
    return 0;
}

