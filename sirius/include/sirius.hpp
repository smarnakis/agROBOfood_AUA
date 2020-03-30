#ifndef _SIRIUS_H_
#define _SIRIUS_H_

/* ROS headers */
#include <ros/ros.h>
#include <ros/package.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>

#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

#include <robot_localization/navsat_conversions.h>
#include <geometry_msgs/PointStamped.h>

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

/* Important definitions */
#define ODOM_TO_WORLD_x -6
#define ODOM_TO_WORLD_y +3
#define ODOM_TO_WORLD_theta 3.14159265
#define PI 3.14159265

/* Global Variables */

extern std::vector <std::pair<double, double>> waypointVect;
extern std::vector<std::pair < double, double> > ::iterator iter; //init. iterator


extern geometry_msgs::PointStamped UTM_point, map_point, UTM_next, map_next;
extern int count, waypointCount, wait_count;
extern double numWaypoints;
extern double latiGoal, longiGoal, latiNext, longiNext;
extern std::string utm_zone;
extern std::string path_local, path_abs;


/* funtions headers*/
int countWaypointsInFile(std::string path_local);
//latitude-longtitude ===> UTM coordinates
geometry_msgs::PointStamped latLongtoUTM(double lati_input, double longi_input);
//UTM ===> to map coordinates (in the odom frame)
geometry_msgs::PointStamped UTMtoMapPoint(geometry_msgs::PointStamped UTM_input);
//Create moveBaseGoal msg for the move base action client.
move_base_msgs::MoveBaseGoal buildGoal(geometry_msgs::PointStamped map_point, geometry_msgs::PointStamped map_next, bool last_point);

#endif