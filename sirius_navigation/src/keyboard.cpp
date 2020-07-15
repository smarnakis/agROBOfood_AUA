#include "sirius_navigation.hpp"
#include <termios.h>

int getch()
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering      
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

  int c = getchar();  // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
  return c;
}


int main(int argc, char *argv[])
{
  // Initialize node and time
	ros::init(argc, argv, "keyboard_node"); //initiate node called collect_gps_waypoints
	ros::NodeHandle n;
	ros::Time::init();

	ros::Publisher pub = n.advertise<std_msgs::Int32>("key_for_collection", 10);
	int c = '+';
	std_msgs::Int32 msg;
	bool flag = false;
	while (ros::ok() or c =='q')
	{
	  c = getch();   // call your non-blocking input function
	  if (c == 'c'){
	    ROS_INFO("c");
	  	msg.data = 1;
	  	pub.publish(msg);
	  }
	  else if (c == 'e'){
	    ROS_INFO("e");
	  	msg.data = 2;
	  	pub.publish(msg);
	  }
/*	  else{
	    ROS_INFO("o");
	  	msg.data = 0;
	  	pub.publish(msg);	
	  }*/
		msg.data = 0;
		pub.publish(msg);		 
		ros::spinOnce();
	}
	ROS_INFO("Keyboad node shutting down...");
	// ros::shutdown();
	return 0;
}