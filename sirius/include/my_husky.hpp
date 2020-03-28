#ifndef _MY_HUSKY_
#define _MY_HUSKY_

/* ROS headers */
#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <std_msgs/String.h>

#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

/* TF headers */
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

/* C++ headers */
#include <queue>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <ctime>
#include <sstream>
#include <cmath>

/* C headers */
#include <assert.h>   // for development phase

#include <stdlib.h>
#include <math.h>

/* Important definitions */
#define ODOM_TO_WORLD_x -6
#define ODOM_TO_WORLD_y +3
#define ODOM_TO_WORLD_theta 3.14159265
#define PI 3.14159265

/* Global Variables */


#endif