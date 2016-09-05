#include <vision/dummy_segmentation_algorithm.hpp>

REGISTER_ALGORITHM_DEF(DummySegmentationAlgorithm);

void DummySegmentationAlgorithm::run()
{
    ROS_INFO("Running dummy segmentation algorithm");
    RawImage::getInstance().getRawRGBImage().copyTo(output_rgb_image_);
    RawImage::getInstance().getRawDepthImage().copyTo(output_depth_image_);
}
