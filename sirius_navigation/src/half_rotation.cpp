#include "sirius_navigation.hpp"

typedef actionlib::SimpleActionClient <move_base_msgs::MoveBaseAction>
MoveBaseClient; //create a type definition for a client called MoveBaseClient



int main(int argc, char** argv)
{
    ros::init(argc, argv, "half_rotation"); //initiate node called gps_waypoint
    ros::NodeHandle n;
    ROS_INFO("Initiated half_rotation node");
    MoveBaseClient ac("/move_base", true);

    while(!ac.waitForServer(ros::Duration(5.0)))
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

    move_base_msgs::MoveBaseGoal goal = half_rotation(); 
    ac.sendGoal(goal); //push goal to move_base node

    //Wait for result
    ac.waitForResult(); //waiting to see if move_base was able to reach goal

    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        ROS_INFO("Husky Rotated!");
        //switch to next waypoint and repeat
    }
    else
    {
        ROS_ERROR("Husky was unable to Rotate!");
        ROS_INFO("Exiting node...");
    }
    ROS_INFO("Husky has performed the requested task successfully!!!\n");
    ROS_INFO("Ending node...");

    // Notify joy_launch_control that waypoint following is complete
    ros::shutdown();
    ros::spin();
    return 0;
}