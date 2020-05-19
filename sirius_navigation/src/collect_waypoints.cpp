#include "sirius_navigation.hpp"


bool collect_request;
bool continue_collection = true;
double x_last,y_last;
int end_btn_num1 = 0, end_btn_num2 = 0, collect_btn_num = 0;
double min_coord_change = 10 * pow(10,-6);

void collect(const sensor_msgs::Joy::ConstPtr& msg){
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
	ros::Subscriber sub_joy = n.subscribe("/joy_teleop/joy", 100, collect);
	ROS_INFO("Initiated collect_waypoints node");

	//Read file path and create/open file
  ros::param::get("/collector/coordinates_file", path_local);
	std::string path_abs =  ros::package::getPath("sirius_navigation") + path_local;
	std::ofstream coordFile (path_abs.c_str());
	ROS_INFO("Saving coordinates to: %s", path_abs.c_str());

	tf::TransformListener listener;
  tf::StampedTransform transform;
	
	if(coordFile.is_open())
	{
		while(continue_collection)
		{
			ros::spinOnce();
			time_current = ros::Time::now();
			if ((collect_request == true) && (time_current - time_last > duration_min))
			{
				bool notDone = true;
				ros::Time time_now;
		  	while(notDone){
				  try
				  {
					  time_now = ros::Time::now();
				  	listener.waitForTransform("map","base_link",time_now,ros::Duration(3.0));
				  	listener.lookupTransform("map","base_link",time_now,transform);
				  	notDone = false;
				  }
			    catch (tf::TransformException& ex)
			    {
			        ROS_WARN("%s", ex.what());
			        ros::Duration(0.01).sleep();
			    }
			  }
			    double x_curr = transform.getOrigin().x();
			    double y_curr = transform.getOrigin().y();

			  	double x_diff = abs((x_curr - x_last)*pow(10,6))*pow(10,-6);
					double y_diff = abs((y_curr - y_last)*pow(10,6))*pow(10,-6);

					if( (x_diff > min_coord_change) || (y_diff > min_coord_change))
					{
						//write waypoint
						ROS_INFO("You have collected another waypoint: X: %f, Y: %f\n",x_curr,y_curr);
/*							ROS_INFO("Press %s button to collect and store another waypoint.", collect_button_sym.c_str());
						ROS_INFO("Press %s button to end waypoint collection.", end_button_sym.c_str());*/
						std::cout << std::endl;
						numWaypoints++;
						coordFile << std::fixed << std::setprecision(8) << x_curr << " " << y_curr << std::endl;
						x_last = x_curr;
						y_last = y_curr;
					}
					else
					{//do not write waypoint
						ROS_WARN("Waypoint not saved, you have not moved enough");
						ROS_WARN("New X: %f   Last X: %f \n", x_curr, x_last );
						ROS_WARN("New Y: %f   Last Y: %f \n", y_curr, y_last );
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