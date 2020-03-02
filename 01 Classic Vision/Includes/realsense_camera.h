#ifndef REALSENSE_CAMERA_H
#define REALSENSE_CAMERA_H

#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include "depth_processor.h"
#include "util.h"

class Realsense_camera
{
public:
    Realsense_camera();
    void capture();
    void capture_fast();
    rs2_intrinsics get_intrinsics();
    void save_image(std::string path, int number);
    void save_intrinsics(std::string filename);
    void save_depth(std::string filename);
    cv::Mat depth_map;
    cv::Mat RGB_image;

    rs2::config cfg;
    rs2::pipeline pipe;
    Depth_processor depth_proces;

    //private:
    rs2_intrinsics intrinsics;
    rs2::frameset allign_frames(rs2::frameset f);
    cv::Mat generate_depth_map(rs2::depth_frame f);
    cv::Mat frame_to_mat(rs2::frame f);
    cv::Mat resize();
};

#endif // REALSENSE_CAMERA_H
