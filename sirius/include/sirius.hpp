#ifndef _SIRIUS_H_
#define _SIRIUS_H_

/* ROS headers */
#include <ros/ros.h>
#include <ros/package.h>
#include <ros/console.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
/* ROS MSGs Headers */
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/PointStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

#include <robot_localization/navsat_conversions.h>


/* TF headers */
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

/* C++ headers */
#include <queue>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <utility>
#include <vector>

#include <ctime>
#include <sstream>
#include <cmath>

/* C headers */
#include <assert.h>   // for development phase
#include <stdlib.h>
#include <math.h>

/* Classes */
class safety
{
private:
	ros::Publisher pub_;
	ros::Subscriber sub_;
public:
	safety(ros::NodeHandle n){
		pub_ = n.advertise<geometry_msgs::Twist>("/emergency_cmd",10);
		sub_ = n.subscribe("/scan",10,&safety::callback,this);
	}
	void callback(const sensor_msgs::LaserScan::ConstPtr& msg){
	  std::vector<float> ran;
	  ran = msg->ranges;
	  geometry_msgs::Twist stop_vel;
	  stop_vel.linear.x = 0.0;
	  stop_vel.linear.y = 0.0;
	  stop_vel.linear.z = 0.0;
	  stop_vel.angular.x = 0.0;
	  stop_vel.angular.y = 0.0;
	  stop_vel.angular.z = 0.0;
	  if ( std::all_of(ran.begin(),ran.end(),[](float i){return i>0.55;}) ){
	    ROS_INFO("ALL GOOD");
	  }
	  else{
	  	pub_.publish(stop_vel);
	    ROS_WARN("SIRIUS IN COLLISION COURSE!!!!");
	  }
	}
};
#endif