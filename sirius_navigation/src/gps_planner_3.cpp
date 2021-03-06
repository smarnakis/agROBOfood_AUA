#include "sirius_navigation.hpp"

typedef actionlib::SimpleActionClient <move_base_msgs::MoveBaseAction>
MoveBaseClient; //create a type definition for a client called MoveBaseClient

int trig_but1,trig_but2;
bool trigger = false;

/* Callbacks */
void resume_motion_callback(const sensor_msgs::Joy::ConstPtr& msg){
  if(msg->buttons[trig_but1]==1 && msg->buttons[trig_but2]==1)
  {
    ROS_WARN("CALLBACK TRIG RECEIVED");
    trigger = true;
  }
  else{
    trigger = false;
  }
}
/*************/

int main(int argc, char** argv)
{
    ros::init(argc, argv, "gps_waypoint"); //initiate node called gps_waypoint
    ros::NodeHandle n;
    ROS_INFO("Initiated gps_waypoint node");
    MoveBaseClient ac("/move_base", true);

    ROS_INFO("WELLCOME to gps_planner_3!!");
    ROS_INFO("This planner awaits a trigger from the joystick to move from keypoint to keypoint");

    //Subscribe to joystick to listen to the motion trigger.
    ros::Subscriber sub_joy = n.subscribe("/joy_teleop/joy", 1, resume_motion_callback);
    ros::param::get("/planner/trigger_button1", trig_but1);
    ros::param::get("/planner/trigger_button2", trig_but2);


    // Initiate publisher to send end of node message
    ros::Publisher pubWaypointNodeEnded = n.advertise<std_msgs::Bool>("/waypoint_following_status", 100);

    //wait for the action server to come up
    while(!ac.waitForServer(ros::Duration(5.0)))
    {
        wait_count++;
        if(wait_count > 3)
        {
            ROS_ERROR("move_base action server did not come up, killing gps_waypoint node...");
            // Notify joy_launch_control that waypoint following is complete
            std_msgs::Bool node_ended;
            node_ended.data = true;
            pubWaypointNodeEnded.publish(node_ended);
            ros::shutdown();
        }
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    //Get Longitude and Latitude goals from text file

    //Count number of waypoints
    ros::param::get("/planner/gps_coordinates_file", path_local);
    // ROS_INFO(path_local);
    numWaypoints = countWaypointsInFile(path_local);

    //Reading waypoints from text file and output results
    waypointVect = getWaypoints(path_local);
	ROS_WARN("Waypoint vector length = %d",int(waypointVect.size()));
    // Iterate through vector of waypoints for setting goals

    for(iter = waypointVect.begin(); iter < (waypointVect.end() - 1); iter++)
    {
        //Setting goal:
        latiCurr = iter->first;
        longiCurr = iter->second;
        bool final_point = false;

        //set next goal point if not at last waypoint
        iter++;
        latiGoal = iter->first;
        longiGoal = iter->second;
        iter--;


        ROS_INFO("Received Latitude goal:%.8f", latiGoal);
        ROS_INFO("Received longitude goal:%.8f", longiGoal);

        //Convert lat/long to utm:
        UTM_point = latLongtoUTM(latiCurr, longiCurr);
        UTM_next = latLongtoUTM(latiGoal, longiGoal);

        //Transform UTM to map point in odom frame
        map_point = UTMtoMapPoint(UTM_point);
        map_next = UTMtoMapPoint(UTM_next);

        //Build goal to send to move_base
        move_base_msgs::MoveBaseGoal goal = buildGoal_2(map_point, map_next); //initiate a move_base_msg called goal

        ROS_WARN("Sirius waits for motion trigger...");
        while(!trigger){
            ros::spinOnce();
        }
        trigger = false;
        ROS_INFO("Motion triggered!! Moving to the next waypoint..");


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
            std_msgs::Bool node_ended;
            node_ended.data = true;
            pubWaypointNodeEnded.publish(node_ended);
            // ros::shutdown();
        }
    } // End for loop iterating through waypoint vector
    
    ROS_INFO("Husky has reached all of its goals!!!\n");
    ROS_INFO("Ending node...");

    // Notify joy_launch_control that waypoint following is complete
    std_msgs::Bool node_ended;
    node_ended.data = true;
    pubWaypointNodeEnded.publish(node_ended);

    ros::shutdown();
    ros::spin();
    return 0;
}