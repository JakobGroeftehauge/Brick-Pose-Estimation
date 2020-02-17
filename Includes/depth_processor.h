#ifndef DEPTH_PROCESSOR_H
#define DEPTH_PROCESSOR_H

#include <librealsense2/rs.hpp>

class Depth_processor
{

public:
    Depth_processor();
    rs2::frame process(rs2::frame input_frame);

//privat:
    rs2::spatial_filter spat_filter;
    rs2::temporal_filter temp_filter;
    rs2::hole_filling_filter hole_filter;
    rs2::disparity_transform depth2disparity;
    rs2::disparity_transform disparity2depth;
};

#endif // DEPTH_PROCESSOR_H
