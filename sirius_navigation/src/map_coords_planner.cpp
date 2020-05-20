#include "sirius_navigation.hpp"

typedef actionlib::SimpleActionClient <move_base_msgs::MoveBaseAction>
MoveBaseClient; 

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "map_coordinates_waypoint"); //initiate node called gps_waypoint
  ros::NodeHandle n;
  ROS_INFO("Initiated map_coordinates_waypoint node");

  MoveBaseClient ac("/move_base", true);
  while(!ac.waitForServer(ros::Duration(5.0)) && ros::ok())
  {
      wait_count++;
      if(wait_count > 3)
      {
          ROS_ERROR("move_base action server did not come up, killing gps_waypoint node...");
          // Notify joy_launch_control that waypoint following is complete
          ros::shutdown();
      }
      ROS_INFO("Waiting for the move_base action server to come up");
  }
  //Get X an Y goals from text file

  //Count number of waypoints
  ros::param::get("/collector/coordinates_file", path_local);
  numWaypoints = countWaypointsInFile(path_local);

  //Reading waypoints from text file and output results
  waypointVect = getWaypoints(path_local);
	
  double XGoal, YGoal, XNext, YNext;

	for(iter = waypointVect.begin(); iter < waypointVect.end(); iter++)
  {
    //Setting goal:
    XGoal = iter->first;
    YGoal = iter->second;
    bool final_point = false;

    //set next goal point if not at last waypoint
    if(iter < (waypointVect.end() - 1))
    {
        iter++;
        XNext = iter->first;
        YNext = iter->second;
        iter--;
    }
    else //set to current
    {
        XNext = iter->first;
        YNext = iter->second;
        final_point = true;
    }

    ROS_INFO("Received X goal:%.8f", XGoal);
    ROS_INFO("Received Y goal:%.8f", YGoal);

    geometry_msgs::PointStamped map_point, map_next;
    map_point.header.frame_id = "map";
    map_point.point.x = XGoal;
    map_point.point.y = YGoal;
    map_next.header.frame_id = "map";
    map_next.point.x = XNext;
    map_next.point.y = YNext;    

		move_base_msgs::MoveBaseGoal goal = buildGoal(map_point, map_next, final_point); //initiate a move_base_msg called goal

    // Send Goal
    ROS_INFO("Sending goal");
    ac.sendGoal(goal); //push goal to move_base node

    //Wait for result
    ac.waitForResult(); //waiting to see if move_base was able to reach goal

    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        ROS_INFO("Husky has reached its goal!");
        //switch to next waypoint and repeat
    }
    else
    {
        ROS_ERROR("Husky was unable to reach its goal. GPS Waypoint unreachable.");
        ROS_INFO("Exiting node...");
        // Notify joy_launch_control that waypoint following is complete
        ros::shutdown();
    }
  }

  ROS_INFO("Husky has reached all of its goals!!!\n");
  ROS_INFO("Ending node...");

  ros::shutdown();
  ros::spin();
  return 0;
}