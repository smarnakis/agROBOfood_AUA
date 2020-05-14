#include "sirius.hpp"

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "dummy_planner");

  ros::NodeHandle n;
  ROS_INFO("Dummy planner node initiated");

  tf::TransformListener listener;
  tf::StampedTransform transform;
  MoveBaseClient ac("/move_base",true);
  while(!ac.waitForServer(ros::Duration(5.0)) && (ros::ok()))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  while(n.ok() && (ros::ok())){
  	ros::Time time_now;
  	bool notDone = true;
  	while(notDone){
		  try
		  {
			  time_now = ros::Time::now();
		  	listener.waitForTransform("odom","base_link",time_now,ros::Duration(3.0));
		  	listener.lookupTransform("odom","base_link",time_now,transform);
		  	notDone = false;
		  	ROS_INFO("Current Position: X,Y,Z\n");
		  }
	    catch (tf::TransformException& ex)
	    {
	        ROS_WARN("%s", ex.what());
	        ros::Duration(0.01).sleep();
	        //return;
	    }
	  }


    move_base_msgs::MoveBaseGoal goal = simpleGoal(transform); //initiate a move_base_msg called goal

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
        ROS_ERROR("Husky was unable to reach its goal. Goal unreachable.");
    }
    ROS_INFO("Exiting node...");
    ros::shutdown();

	}	
}