#include "my_husky.hpp"

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))&&(ros::ok())){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;
  //obom frame (6,-3)
  //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "map";
  
  int Num_points = 7;
  float x[] = {-5.0,4.5,4.5,-5,-5,4.5,4.5};
  float y[] = {0.9,0.9,2.6,2.6,5,5,-2};
  float or_z[] = {1,-0.707,0,-0.707,1,0.707,0};
  float w[] = {0,0.707,1,0.707,0,0.707,1};

  while(ros::ok()){
    for (int i = 0; i < Num_points; ++i){
      goal.target_pose.header.stamp = ros::Time::now();
      goal.target_pose.pose.position.x = cos(ODOM_TO_WORLD_theta)*(ODOM_TO_WORLD_x + x[i]);
      goal.target_pose.pose.position.y = cos(ODOM_TO_WORLD_theta)*(ODOM_TO_WORLD_y + y[i]);
      goal.target_pose.pose.orientation.z = or_z[i];
      goal.target_pose.pose.orientation.w = w[i];
      ROS_INFO("Sending goal");
      ac.sendGoal(goal);
      ROS_INFO("Travelling to access point...");
      ac.waitForResult();
      if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
          ROS_INFO("Reached access point, proceeding to the next\n");
      }
      else{
        goal.target_pose.header.stamp = ros::Time::now();
        goal.target_pose.pose.position.x = 0.0;
        goal.target_pose.pose.position.y = 0.0;
        goal.target_pose.pose.orientation.w = 1.0;
        ac.sendGoal(goal);
        ROS_INFO("Failed to reach requested access point, Returning to Base...");
        ac.waitForResult();
        ROS_INFO("Reached Base, Planner powering down...");
      }
    }
  }
  return 0;
}