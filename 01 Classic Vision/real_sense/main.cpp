#include <iostream>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include "../Includes/depth_processor.h"
#include "../Includes/realsense_camera.h"

using namespace std;
const string img_folder = "../../03 Data/Dataset3_oldbricks/";

int main()
{
    //float pixel[2] = {500,600};
    //float point[3];
    int img_counter = 32;
    Realsense_camera camera;
    //camera.capture();
    //cout << "intrinsics: \nfx:  " << camera.get_intrinsics().fx << " fy: " << camera.get_intrinsics().fy << endl; 
    //cout << "ppx: " << camera.get_intrinsics().ppx << " ppy: " << camera.get_intrinsics().ppy << endl;
    //rs2_deproject_pixel_to_point(point, &camera.intrinsics, pixel, camera.depth_map.at<double>(pixel[0], pixel[1]));
    //cout << "x: " << point[0] << " y: " << point[1] << " z: " << point[2] << endl;
    //cv::imshow("RGB", camera.RGB_image);
    //cv::imshow("Depth", camera.depth_map);
    const auto window_name = "Display Image";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::Mat R = cv::Mat(50, 50, CV_8UC3);
    randu(R, cv::Scalar::all(0), cv::Scalar::all(255));
    char keyPressed = 0;
    while (keyPressed != 'q' && cv::getWindowProperty(window_name, cv::WND_PROP_AUTOSIZE) >= 0)
    {
        camera.capture_fast();
        cv::imshow(window_name, camera.RGB_image);
        keyPressed = cv::waitKey(5);
        if (keyPressed == 'c')
        {
            camera.save_image(img_folder,img_counter);
            //cv::imwrite(img_folder + "capture_" + to_string(img_counter) + ".png",camera.RGB_image);
            std::cout << "Image saved" << endl;
            img_counter++;
        }
           
    }

   /* if (cv::waitKey(10000) == 'a')
        std::cout << "pressed a" << endl;*/


    //cv::waitKey(0);
    return 0;
}
/*
int main()
{
    cv::Mat depth_edge;
    Realsense_camera camera;
    const auto window_name = "Display Image";
    cv::Mat depth_converted;
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    while (cv::waitKey(20) < 0 && cv::getWindowProperty(window_name, cv::WND_PROP_AUTOSIZE) >= 0)
    {
        camera.capture();
        depth_converted = camera.depth_map.clone();
        cv::normalize(depth_converted, depth_converted, 0, 255, cv::NORM_MINMAX, CV_8U);
        //depth_converted.convertTo(depth_converted, CV_8U);
        cv::Canny(depth_converted, depth_edge, 20, 110);
        cv::cvtColor(depth_edge, depth_edge, cv::COLOR_GRAY2BGR);
        cv::bitwise_or(camera.RGB_image, depth_edge, depth_edge);
        cv::imshow(window_name, depth_edge);
        //cv::imshow("RGB", camera.RGB_image);
        //cv::imshow("Depth", camera.depth_map / 2);
        //cv::waitKey(300);
    }

    return 0;
}

*/
