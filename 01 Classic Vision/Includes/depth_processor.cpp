#include "depth_processor.h"
#include <librealsense2/rs.hpp>

#include <iostream>
Depth_processor::Depth_processor()
{
    /*Configure Spatial filter
     * @alpha - weight of current pixel [25..100%]
     * @theta - defines the depth gradient below which the smoothing will occur as number of depth levels
     * @magnitude - Number of iterations
     * @hole_fill - an in-place heuristic symmetric hole-filling mode applied horizontally during the filter passes.
     *              Intended to rectify minor artefacts with minimal performance impact
     */
    float alpha = 0.8 , delta = 1, magnitude = 2;
    float hole_fill = 0;
    this->spat_filter = rs2::spatial_filter(alpha, delta, magnitude, hole_fill);
    //this->spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.8f);
    //this->spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 1.0f); //
    //this->spat_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, 2.0f); //Number of filter iterations

    /*Configure Temporal Filter
     *
     * @alpha - weight of current frame
     * @delta - threshold for edge classification
     * @persistence_control - predefined rules for when a missing pixel value can be replaced with past values
     * See page for more info: https://intelrealsense.github.io/librealsense/doxygen/classrs2_1_1temporal__filter.html
     */
    alpha = 0.2 , delta = 1;
    int persistence_control = 4;
    this->temp_filter = rs2::temporal_filter(alpha, delta, persistence_control);

    /*Setup hole_filling_filter
     * @Mode: How to fill in missing pixels
     *   0 - Fill from left.
     *   1 - Neighbour pixel furthest from sensor.
     *   2 - Neighbour pixel closest to sensor.
     */
    int mode = 1;
    this->hole_filter = rs2::hole_filling_filter(mode);


    this->depth2disparity = rs2::disparity_transform(true); //The boolean variable defines which direction the transform goes.
    this->disparity2depth = rs2::disparity_transform(false); // True - to disparity. False - from disparity.

}

rs2::frame Depth_processor::process(rs2::frame input_frame)
{
    rs2::frame disparity_frame = depth2disparity.process(input_frame);
    rs2::frame spat_disp = spat_filter.process(disparity_frame);
    rs2::frame temp_disp = temp_filter.process(spat_disp);
    rs2::frame depth_frame = disparity2depth.process(temp_disp);
    depth_frame = hole_filter.process(depth_frame);
    return depth_frame;
}
