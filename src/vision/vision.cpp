/**
 * @file   vision.cpp
 * @author Gabriel Naves da Silva
 * @date   27/02/2016
 *
 * @attention Copyright (C) 2014 UnBall Robot Soccer Team
 *
 * @brief Vision class
 */

#include <vision/vision.hpp>

Vision& Vision::getInstance()
{
    static Vision vision;
    return vision;
}

/**
 * Starts the vision system.
 */
Vision::Vision()
{
    has_received_first_image_ = false;
    ConfigParser::getInstance().parseConfigFile();
    Segmenter::getInstance().init();
}

/**
 * Execute vision processing. Nothing will be done until the first image has been
 * received.
 */
void Vision::run()
{
    if (has_received_first_image_)
    {
        Segmenter::getInstance().runSegmentationAlgorithms();
        Identifier::getInstance().runIdentificationAlgorithms();
        Tracker::getInstance().runTracking();

        VisionGUI::getInstance().showOutputImage();
    }
}

/**
 * Receives the raw image, and passes it on to the RawImage class and the VisionGUI.
 */
void Vision::setRawImage(const cv::Mat &raw_image)
{
    if (isValidSize(raw_image))
    {
        has_received_first_image_ = true;
        RawImage::getInstance().setRawImage(raw_image);
        VisionGUI::getInstance().setInitialImage(raw_image);
    }
}

/**
 * Check whether an image has a valid size, i.e., neither the width nor the height can be zero.
 * @return true if the image has proper size, false otherwise.
 */
bool Vision::isValidSize(const cv::Mat &img)
{
    return not (img.rows == 0 or img.cols == 0);
}
