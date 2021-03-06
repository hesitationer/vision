/**
 * @file   tracker.hpp
 * @author Gabriel Naves da Silva
 * @date   22/03/2016
 *
 * @attention Copyright (C) 2014 UnBall Robot Soccer Team
 *
 * @brief  Tracker class
 *
 * The tracker class manages the objects that are being tracked.
 */

#ifndef VISION_TRACKER_H_
#define VISION_TRACKER_H_

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include <ros/ros.h>

#include <vision/tracked_object.hpp>
#include <vision/tracking_output.hpp>

class Tracker
{
  public:
    static Tracker& getInstance();

    void runTracking();

    void addTrackedObject(std::shared_ptr<TrackedObject> obj);

    std::unordered_map<std::string, TrackingOutput> getTrackerOutput();

  private:
    std::vector<std::shared_ptr<TrackedObject>> tracked_objects_;
};

#endif // VISION_TRACKER_H_
