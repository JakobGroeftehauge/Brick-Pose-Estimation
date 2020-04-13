#include "realsense_camera.h"
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include "depth_processor.h"

Realsense_camera::Realsense_camera()
{ 
    this->cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 6);
    this->cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 6);
    this->pipe.start(this->cfg);
    //this->pipe.start();
}

void Realsense_camera::capture()
{
    rs2::frameset frames;
    rs2::frame depth_frame;

    //Capture X images to ensure the auto exposure has configured.
    for(int i = 0; i < 10; i++)
    {
        frames = this->pipe.wait_for_frames();
        frames = allign_frames(frames);

        //Perform post processing on the depth frame every capturing, inorder to take advantageous
        //of the temporal diffenrence filter. (Only a thesis, should maybe be tested).
        depth_frame = this->depth_proces.process(frames.get_depth_frame());
    }   
    this->intrinsics = depth_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data
    //Perform post processing on captured images, and store them.
    this->depth_map = generate_depth_map(depth_frame);
    std::cout << "RGB" << std::endl;
    this->RGB_image = frame_to_mat(frames.get_color_frame());
}

void Realsense_camera::capture_fast()
{
    rs2::frameset frames;
    rs2::frame depth_frame;
    //Capture only a single image
    frames = this->pipe.wait_for_frames();
    frames = allign_frames(frames);
    depth_frame = this->depth_proces.process(frames.get_depth_frame());
    this->intrinsics = depth_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data
    //Perform post processing on captured images, and store them.
    this->depth_map = generate_depth_map(depth_frame);
    //std::cout << "RGB" << std::endl;
    this->RGB_image = frame_to_mat(frames.get_color_frame());
}

rs2_intrinsics Realsense_camera::get_intrinsics()
{
    //auto depth = f.get_depth_frame();
    //rs2_intrinsics intr = depth.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data
	return this->intrinsics;
}

void Realsense_camera::save_image(std::string path, int number)
{
    cv::imwrite(path + "OB_colorIMG_" + std::to_string(number) + ".png", this->RGB_image);
    save_intrinsics(path + "OB_intrinsics_" + std::to_string(number) + ".csv");
    save_depth(path + "OB_depth_" + std::to_string(number) + ".csv");

}

void Realsense_camera::save_intrinsics(std::string filename)
{
    std::ofstream csv;

    csv.open(filename);

    csv << "Width, " << this->intrinsics.width << "\n";
    csv << "Height, " << this->intrinsics.height << "\n";
    csv << "ppx, " << this->intrinsics.ppx << "\n";
    csv << "ppy, " << this->intrinsics.ppy << "\n";
    csv << "fx, " << this->intrinsics.fx << "\n";
    csv << "fy, " << this->intrinsics.fy << "\n";

    csv.close();

}

void Realsense_camera::save_depth(std::string filename)
{
    std::ofstream file;
    file.open(filename);

    for (int w = 0; w < this->depth_map.size().width; w++)
    {
        for (int h = 0; h < this->depth_map.size().height; h++)
        {
            file << (double)this->depth_map.at<double>(cv::Point(w, h)) << ", ";
        }

        file << "\n";
    }

    file.close();
}

rs2::frameset Realsense_camera::allign_frames(rs2::frameset frames)
{
    rs2::align align(RS2_STREAM_COLOR);
    //rs2::align align(RS2_STREAM_DEPTH); //<- used for testing
    rs2::frameset aligned_frames = align.process(frames);
    return aligned_frames;
}

cv::Mat Realsense_camera::generate_depth_map(rs2::depth_frame f)
{
    cv::Mat dm = frame_to_mat(f);
    dm.convertTo(dm, CV_64F);
    auto depth_scale = this->pipe.get_active_profile().get_device().first<rs2::depth_sensor>().get_depth_scale();
    dm = dm * depth_scale;
    return dm;

}

cv::Mat Realsense_camera::frame_to_mat(rs2::frame f)
{
    auto vf = f.as<rs2::video_frame>();
        const int w = vf.get_width();
        const int h = vf.get_height();

        if (f.get_profile().format() == RS2_FORMAT_BGR8)
        {
            //std::cout << "1" << std::endl;
            return cv::Mat(cv::Size(w, h), CV_8UC3, (void*)f.get_data(), cv::Mat::AUTO_STEP);
        }
        else if (f.get_profile().format() == RS2_FORMAT_RGB8)
        {
            //std::cout << "2" << std::endl;
            auto r = cv::Mat(cv::Size(w, h), CV_8UC3, (void*)f.get_data(), cv::Mat::AUTO_STEP);

            cv::Mat r_BGR;
            cv::cvtColor(r, r_BGR, CV_RGB2BGR);
            return r_BGR;
            /*cv::imshow("R_RGB", r_BGR);
            cv::imshow("r", r);
            cv::waitKey(0);*/

        }
        else if (f.get_profile().format() == RS2_FORMAT_Z16)
        {
            //std::cout << "3" << std::endl;
            return cv::Mat(cv::Size(w, h), CV_16UC1, (void*)f.get_data(), cv::Mat::AUTO_STEP);
        }
        else if (f.get_profile().format() == RS2_FORMAT_Y8)
        {
            //std::cout << "4" << std::endl;
            return cv::Mat(cv::Size(w, h), CV_8UC1, (void*)f.get_data(), cv::Mat::AUTO_STEP);
        }

        throw std::runtime_error("Frame format is not supported yet!");
}
