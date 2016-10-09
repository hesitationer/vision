/**
 * @file   tracked_object.hpp
 * @author Gabriel Naves da Silva
 * @date   22/03/2016
 *
 * @attention Copyright (C) 2014 UnBall Robot Soccer Team
 *
 * @brief  TrackedObject class
 */

#include <vision/tracked_object.hpp>
#include <cstdio>

TrackedObject::TrackedObject(std::string name)
{
    name_ = name;
    counter_ = 0;
    weight_ = 0.3;
    last_pose_vector_ = std::vector<cv::Point2f>();
}

std::vector<cv::Point2f> TrackedObject::getPositionVector(){
    return position_;
}

void TrackedObject::runTracking()   
{
    auto id_output = identification_algorithm_->getIdentificationOutput();
    auto pose_vector = id_output->object_pose;
    cv::Mat rgb_output = VisionGUI::getInstance().getOutputRGBImage();
        
    if (last_pose_vector_.size() == 0)
        last_pose_vector_ = pose_vector;
    std::vector<float> module(last_pose_vector_.size());
    std::vector<float> module_aux(last_pose_vector_.size());
    std::vector<trackParams> params(last_pose_vector_.size());
    for (int j = 0; j < last_pose_vector_.size(); j++)
    {
        for (int i = 0; i < pose_vector.size(); i++)
        {
            if (pose_vector[i].x != -1 && pose_vector[i].y != -1)
            {
                auto vector_ = cv::Point2f(last_pose_vector_[j].x - pose_vector[i].x, last_pose_vector_[j].y - pose_vector[i].y);
                module[i] = sqrt(pow(vector_.x, 2) + pow(vector_.y, 2));
                module_aux[i] = module[i];
            }
        }
        auto minDist = *std::min_element(module_aux.begin(), module_aux.end());
        for (int i = 0; i < module.size(); i++){
            if(module[i] == minDist){
                last_pose_vector_[j] = pose_vector[i];
                module_aux[i] = 1000000;
                pose_vector[i] = cv::Point2f(-1, -1);
            }
        }

        // if (last_pose_vector_[j].x != -1 || last_pose_vector_[j].y != -1)
        // {
        //     update(&params[j], last_pose_vector_[j]);
        // }else
        // {
        //     counter_++;
        //     predict(&params[j]);
        //     if (counter_ > 15)
        //     {
        //         resetLastPose(&params[j]);
        //         ROS_INFO("HERE");
        //         counter_ = 0;
        //     }

        // }
        // predict(&params[j]);

        if(name_ == "ball"){
            cv::circle(rgb_output, last_pose_vector_[j], 10, cv::Scalar(255,0,0));
        }else if (name_ == "our_robots")
        {
            cv::Point point1 = cv::Point(last_pose_vector_[j].x-10, last_pose_vector_[j].y-10);
            cv::Point point2 = cv::Point(last_pose_vector_[j].x+10, last_pose_vector_[j].y+10);
            cv::rectangle(rgb_output, point1, point2, cv::Scalar(133, 133, 133), 3, 8, 0); 
        }
        else if (name_ == "opponent_robots")
        {   
            cv::Point point1 = cv::Point(last_pose_vector_[j].x-10, last_pose_vector_[j].y-10);
            cv::Point point2 = cv::Point(last_pose_vector_[j].x+10, last_pose_vector_[j].y+10);
            cv::rectangle(rgb_output, point1, point2, cv::Scalar(133, 0, 133), 3, 8, 0); 
        }
    }
   
}

bool TrackedObject::isName(std::string name)
{
    return name == name_;
}

void TrackedObject::setIdentificationAlgorithm(std::shared_ptr<IdentificationAlgorithm> id_alg)
{
    identification_algorithm_ = id_alg;
}


void TrackedObject::predict(trackParams *param)
{
    param->previous_pose_ = param->predicted_pose_;

    param->predicted_pose_ += param->predicted_velocity_*1.25;
}

void TrackedObject::update(trackParams *param, cv::Point2f measured_pose)
{
    param->previous_pose_ = param->predicted_pose_;
    cv::Point2f pose_discrepancy;

    param->predicted_pose_ = weight_ * param->predicted_pose_ +  (1 - weight_) * measured_pose;
    param->predicted_velocity_ = (param->predicted_pose_ - param->previous_pose_);

}  

void TrackedObject::resetFilter(trackParams *param)
{
    param->predicted_velocity_ = cv::Point2f(0,0);
    param->predicted_pose_ = cv::Point2f(320, 240);
}

void TrackedObject::resetLastPose(trackParams *param)
{
    param->predicted_velocity_ = cv::Point2f(0,0);
    param->predicted_pose_ = cv::Point2f(param->previous_pose_);
}

bool TrackedObject::isOutOfLimits(cv::Point2f position)
{
    if(position.x < 0 or position.y < 0 or
       position.x > 640 or position.y > 480)
        return true;
    else 
        return false;
}
