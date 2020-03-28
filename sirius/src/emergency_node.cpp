#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <ros/console.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>

std_msgs::Bool stop_msg;

void emergency_stopCallback(const std_msgs::Bool::ConstPtr& msg){
	stop_msg.data = msg->data;
  ROS_INFO("Callback: %d\n",stop_msg.data);
}

int main(int argc, char** argv){
  ros::init(argc, argv, "emergency_stop_node");

  ros::NodeHandle nh;
  ros::Publisher stop_pub = nh.advertise<std_msgs::Bool>("e_stop",10);
  bool stop = false;
  ros::Subscriber stop_sub = nh.subscribe("emergency_stop",10,emergency_stopCallback);

  stop_msg.data = stop;
  //ros::Rate loop_rate(0.1);
  while(ros::ok()){
    ROS_INFO("Loop: %d\n",stop_msg.data);
  	stop_pub.publish(stop_msg);
    //loop_rate.sleep();
    ros::spinOnce();
  }

  return 0;
}