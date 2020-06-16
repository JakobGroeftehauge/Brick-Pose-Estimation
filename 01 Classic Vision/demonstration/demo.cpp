#include <iostream>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include "../Includes/depth_processor.h"
#include "../Includes/realsense_camera.h"
#include "../Includes/Chamfer_brick_detector.h"
#include "../Includes/Brick_Detector.h"
#include "../Includes/util.h"

int main()
{
    Realsense_camera camera;
    const auto window_name = "Display Image";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::Mat R = cv::Mat(50, 50, CV_8UC3);
    randu(R, cv::Scalar::all(0), cv::Scalar::all(255));
    char keyPressed = 0;

    //--------------- LINE BASED ------------------
    Brick_Detector line_based_detector;
    line_based_detector.set_brick_specs(96, 21, 108, 28);
    //line_based_detector.set_brick_specs(96 * 1.5, 21 * 1.5, 108 * 1.5, 28 * 1.5);
    line_based_detector.set_canny_thresh(48, 48+84);

    //--------------- CHAMFER MATCHING -------------
    Chamfer_brick_detector chamfer_detector;
    chamfer_detector.set_brick_specs(96, 21, 108, 28);
    chamfer_detector.set_brick_specs(96*1.5, 21* 1.5, 108* 1.5, 28* 1.5);
    chamfer_detector.set_NMS_thresh(200);
    chamfer_detector.set_canny_thresh(25, 70);
    chamfer_detector.set_resolution(20,3);


    while (keyPressed != 'q' && cv::getWindowProperty(window_name, cv::WND_PROP_AUTOSIZE) >= 0)
    {
        camera.capture_fast();
        cv::Mat clone = camera.RGB_image.clone();
        chamfer_detector.detect(clone);
        //line_based_detector.detect(clone);
        //util::print_rotated_bounding_boxes(camera.RGB_image, line_based_detector.predictions);
        util::print_rotated_bounding_boxes(clone, chamfer_detector.predictions);
        cv::imshow(window_name, clone);
        keyPressed = cv::waitKey(5);
    }

    return 0;
}