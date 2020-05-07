#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <ros/console.h>


void safetyCallback(const sensor_msgs::LaserScan::ConstPtr& msg){
/*	float max_range = msg->range_max;
  float angle_min,angle_max,angle_inc;
  int measurements;
  angle_min = msg->angle_min;
  angle_max = msg->angle_max;
  angle_inc = msg->angle_increment;
  measurements = (angle_max - angle_min) / angle_inc;
  float ranges[measurements];*/
  std::vector<float> ran;
  ran = msg->ranges;
  if ( std::all_of(ran.begin(),ran.end(),[](float i){return i>1;}) ){
    ROS_INFO("ALL GOOD");
  }
  else{
    ROS_WARN("SIRIUS IN COLLISION COURSE!!!!");
  }
/*
  ROS_INFO("%f | %f | %f\n",angle_min,angle_max,angle_inc);
  ROS_INFO("%f\n",ran[100]);*/
}

int main(int argc, char** argv){
  ros::init(argc, argv, "safety_node");

  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("/scan",10,safetyCallback);

  ros::spin();

  return 0;
}