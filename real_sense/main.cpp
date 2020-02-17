#include <iostream>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include "depth_processor.h"
#include "realsense_camera.h"

using namespace std;

int main()
{

    Realsense_camera camera;
    camera.capture();

    cv::imshow("RGB", camera.RGB_image);
    cv::imshow("Depth", camera.depth_map);
    cv::waitKey(0);
    return 0;
}
