// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API

// 3rd party header for writing png files
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

rs2::frame depth;

rs2::frame color;

int counter = 67;

bool save_frame_raw_data(const std::string& filename, rs2::frame frame)
{
    bool ret = false;
    auto image = frame.as<rs2::video_frame>();
    if (image)
    {
        std::ofstream outfile(filename.data(), std::ofstream::binary);
        outfile.write(static_cast<const char*>(image.get_data()), image.get_height()* image.get_stride_in_bytes());

        outfile.close();
        ret = true;
    }

    return ret;
}

void mouseCallback(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        rs2::video_frame v_depth = depth.as<rs2::video_frame>();
        std::stringstream png_file;
        png_file << std::to_string(counter) + "rs-save-to-disk-output-" << v_depth.get_profile().stream_name() << ".png";
        stbi_write_png(png_file.str().c_str(), v_depth.get_width(), v_depth.get_height(),
            v_depth.get_bytes_per_pixel(), v_depth.get_data(), v_depth.get_stride_in_bytes());
        std::cout << "Saved " << png_file.str() << std::endl;

        save_frame_raw_data(std::to_string(counter)+"1.bin", depth);

        std::stringstream png_file2;
        rs2::video_frame v_color = color.as<rs2::video_frame>();
        png_file2 << std::to_string(counter) + "rs-save-to-disk-output-" << v_color.get_profile().stream_name() << ".png";
        stbi_write_png(png_file2.str().c_str(), v_color.get_width(), v_color.get_height(),
            v_color.get_bytes_per_pixel(), v_color.get_data(), v_color.get_stride_in_bytes());
        std::cout << "Saved " << png_file2.str() << std::endl;

        counter++;
    }
}

int main(int argc, char* argv[]) try
{
    rs2::config cfg;
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();

    //using namespace cv;
    const auto depth_window_name = "Depth Image";
    const auto RGB_window_name = "RGB Image";
    cv::namedWindow(depth_window_name, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(RGB_window_name, cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback(RGB_window_name, mouseCallback, NULL);

    while (cv::waitKey(1) < 0 && cv::getWindowProperty(depth_window_name, cv::WND_PROP_AUTOSIZE) >= 0)
    {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
        depth = data.get_depth_frame().apply_filter(color_map);
        color = data.get_color_frame().apply_filter(color_map);

        // Query frame size (width and height)
        const int w_d = depth.as<rs2::video_frame>().get_width();
        const int h_d = depth.as<rs2::video_frame>().get_height();

        const int w_c = color.as<rs2::video_frame>().get_width();
        const int h_c = color.as<rs2::video_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data
        cv::Mat depth_image(cv::Size(w_d, h_d), CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);
        cv::Mat RGB_image(cv::Size(w_c, h_c), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);

        cv::cvtColor(depth_image, depth_image, CV_BGR2RGB);
        cv::cvtColor(RGB_image, RGB_image, CV_BGR2RGB);

        // Update the window with new data
        cv::imshow(depth_window_name, depth_image);
        cv::imshow(RGB_window_name, RGB_image);
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception & e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}