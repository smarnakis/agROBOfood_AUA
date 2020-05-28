#include "sirius_navigation.hpp"


bool collect_request;
bool continue_collection = true;
double lat_curr=0,long_curr=0,lat_last=0,long_last=0;
int end_btn_num1 = 0, end_btn_num2 = 0, collect_btn_num = 0;
double min_coord_change = 10 * pow(10,-6);

void collect_CB(const sensor_msgs::Joy::ConstPtr& msg){
	if(msg->buttons[collect_btn_num]==1)
	{
		collect_request = true;
	}
	else
	{
		collect_request = false;
	}

	if(msg->buttons[end_btn_num1]==1 && msg->buttons[end_btn_num2]==1)
	{
		continue_collection = false;
	}
}


void gps_CB(const sensor_msgs::NavSatFix::ConstPtr& gps_msg)
{
		lat_curr = gps_msg->latitude;
		long_curr = gps_msg->longitude;
}


int main(int argc, char *argv[])
{
	int numWaypoints = 0;
	std::string path_local;

  // Initialize node and time
	ros::init(argc, argv, "collect_waypoints"); //initiate node called collect_gps_waypoints
	ros::NodeHandle n;
	ros::Time::init();
	ros::Time time_last;
	ros::Time time_current;
	ros::Duration duration_min(1);

// Get button numbers to collect waypoints and end collection
	ros::param::get("/collector/collect_button_num", collect_btn_num);
	ros::param::get("/collector/end_button_num1", end_btn_num1);
	ros::param::get("/collector/end_button_num2", end_btn_num2);

  //Initiate subscribers
	ros::Subscriber sub_joy = n.subscribe("/joy_teleop/joy", 100, collect_CB);
	ros::Subscriber sub_gps = n.subscribe("/gps/filtered", 100, gps_CB);
	ROS_INFO("Initiated collect_waypoints node");

	//Read file path and create/open file
  ros::param::get("/collector/gps_coordinates_file", path_local);
	std::string path_abs =  ros::package::getPath("sirius_navigation") + path_local;
	std::ofstream coordFile (path_abs.c_str());
	ROS_INFO("Saving coordinates to: %s", path_abs.c_str());
	
	if(coordFile.is_open())
	{
		while(continue_collection)
		{
			ros::spinOnce();
			time_current = ros::Time::now();
			if ((collect_request == true) && (time_current - time_last > duration_min))
			{
		  	double lat_diff = abs((lat_curr - lat_last)*pow(10,6))*pow(10,-6);
				double long_diff = abs((long_curr - long_last)*pow(10,6))*pow(10,-6);

				if( (lat_diff > min_coord_change) || (long_diff > min_coord_change))
				{
					//write waypoint
					ROS_INFO("You have collected another waypoint: X: %f, Y: %f\n",lat_curr,long_curr);
/*							ROS_INFO("Press %s button to collect and store another waypoint.", collect_button_sym.c_str());
					ROS_INFO("Press %s button to end waypoint collection.", end_button_sym.c_str());*/
					std::cout << std::endl;
					numWaypoints++;
					coordFile << std::fixed << std::setprecision(8) << lat_curr << " " << long_curr << std::endl;
					lat_last = long_curr;
					lat_last = long_curr;
				}
				else
				{//do not write waypoint
					ROS_WARN("Waypoint not saved, you have not moved enough");
					ROS_WARN("New X: %f   Last X: %f \n", lat_curr, lat_last );
					ROS_WARN("New Y: %f   Last Y: %f \n", long_curr, long_last );
				}
				time_last = time_current;
			}
			else{}
			ros::spinOnce();
		}

		coordFile.close();
		ROS_INFO("End request registered.");
	}
	else
	{
		ROS_ERROR("Unable to open file.");
		ROS_INFO("Exiting..");
	}

	ROS_INFO("Closed waypoint file, you have collected %d waypoints.", numWaypoints);
	ROS_INFO("Ending node...");

	ros::shutdown();
	return 0;

}