#include <sirius_navigation.hpp>

std::vector <std::pair<double, double>> waypointVect;
std::vector<std::pair < double, double> > ::iterator iter; //init. iterator

geometry_msgs::PointStamped UTM_point, map_point, UTM_next, map_next;
int count = 0, waypointCount = 0, wait_count = 0;
double numWaypoints = 0;
double latiGoal, longiGoal, latiNext, longiNext,latiCurr,longiCurr;
std::string utm_zone;
std::string path_local, path_abs;

int countWaypointsInFile(std::string path_local)
{
    path_abs = ros::package::getPath("sirius_navigation") + path_local;
    std::ifstream fileCount(path_abs.c_str());
    if(fileCount.is_open())
    {
        double lati = 0;
        while(!fileCount.eof())
        {
            fileCount >> lati;
            ++count;
        }
        count = count - 1;
        numWaypoints = count / 2;
        ROS_INFO("%.0f map coordinate waypoints were read", numWaypoints);
        fileCount.close();
    }
    else
    {
        std::cout << "Unable to open waypoint file" << std::endl;
        ROS_ERROR("Unable to open waypoint file");
    }
    return numWaypoints;
}

std::vector <std::pair<double, double>> getWaypoints(std::string path_local)
{
    double lati = 0, longi = 0;

    path_abs = ros::package::getPath("sirius_navigation") + path_local;
    std::ifstream fileRead(path_abs.c_str());
    for(int i = 0; i < numWaypoints; i++)
    {
        fileRead >> lati;
        fileRead >> longi;
        waypointVect.push_back(std::make_pair(lati, longi));
    }
    fileRead.close();

    //Outputting vector
    ROS_INFO("The following Map coordinate Waypoints have been set:");
    for(std::vector < std::pair < double, double >> ::iterator iterDisp = waypointVect.begin(); iterDisp != waypointVect.end();
    iterDisp++)
    {
        ROS_INFO("%.9g %.9g", iterDisp->first, iterDisp->second);
    }
    return waypointVect;
}

geometry_msgs::PointStamped latLongtoUTM(double lati_input, double longi_input)
{
    double utm_x = 0, utm_y = 0;
    geometry_msgs::PointStamped UTM_point_output;

    //convert lat/long to utm
    RobotLocalization::NavsatConversions::LLtoUTM(lati_input, longi_input, utm_y, utm_x, utm_zone);

    //Construct UTM_point and map_point geometry messages
    UTM_point_output.header.frame_id = "utm";
    UTM_point_output.header.stamp = ros::Time(0);
    UTM_point_output.point.x = utm_x;
    UTM_point_output.point.y = utm_y;
    UTM_point_output.point.z = 0;

    return UTM_point_output;
}

geometry_msgs::PointStamped UTMtoMapPoint(geometry_msgs::PointStamped UTM_input)
{
    geometry_msgs::PointStamped map_point_output;
    bool notDone = true;
    tf::TransformListener listener; //create transformlistener object called listener
    ros::Time time_now = ros::Time::now();
    while(notDone)
    {
        try
        {
            UTM_point.header.stamp = ros::Time::now();
            listener.waitForTransform("odom", "utm", time_now, ros::Duration(3.0));
            listener.transformPoint("odom", UTM_input, map_point_output);
            notDone = false;
        }
        catch (tf::TransformException& ex)
        {
            ROS_WARN("%s", ex.what());
            ros::Duration(0.01).sleep();
            //return;
        }
    }
    return map_point_output;
}

move_base_msgs::MoveBaseGoal buildGoal(geometry_msgs::PointStamped map_point, geometry_msgs::PointStamped map_next, bool last_point)
{
    move_base_msgs::MoveBaseGoal goal;

    //Specify what frame we want the goal to be published in
    goal.target_pose.header.frame_id = "odom";
    goal.target_pose.header.stamp = ros::Time::now();

    // Specify x and y goal
    goal.target_pose.pose.position.x = map_point.point.x; //specify x goal
    goal.target_pose.pose.position.y = map_point.point.y; //specify y goal

    // Specify heading goal using current goal and next goal (point robot towards its next goal once it has achieved its current goal)
    if(last_point == false)
    {
        tf::Matrix3x3 rot_euler;
        tf::Quaternion rot_quat;

        // Calculate quaternion
        float x_curr = map_point.point.x, y_curr = map_point.point.y; // set current coords.
        float x_next = map_next.point.x, y_next = map_next.point.y; // set coords. of next waypoint
        float delta_x = x_next - x_curr, delta_y = y_next - y_curr;   // change in coords.
        float yaw_curr = 0, pitch_curr = 0, roll_curr = 0;
        yaw_curr = atan2(delta_y, delta_x);

        // Specify quaternions
        rot_euler.setEulerYPR(yaw_curr, pitch_curr, roll_curr);
        rot_euler.getRotation(rot_quat);

        goal.target_pose.pose.orientation.x = rot_quat.getX();
        goal.target_pose.pose.orientation.y = rot_quat.getY();
        goal.target_pose.pose.orientation.z = rot_quat.getZ();
        goal.target_pose.pose.orientation.w = rot_quat.getW();
    }
    else
    {
        goal.target_pose.pose.orientation.w = 1.0;
    }

    return goal;
}


move_base_msgs::MoveBaseGoal buildGoal_2(geometry_msgs::PointStamped map_point_curr, geometry_msgs::PointStamped map_goal)
{
    move_base_msgs::MoveBaseGoal goal;

    //Specify what frame we want the goal to be published in
    goal.target_pose.header.frame_id = "odom";
    goal.target_pose.header.stamp = ros::Time::now();

    // Specify x and y goal
    goal.target_pose.pose.position.x = map_goal.point.x; //specify x goal
    goal.target_pose.pose.position.y = map_goal.point.y; //specify y goal

    // Specify heading goal using current goal and next goal (point robot towards its next goal once it has achieved its current goal)
    tf::Matrix3x3 rot_euler;
    tf::Quaternion rot_quat;

    // Calculate quaternion
    float x_curr = map_point_curr.point.x, y_curr = map_point_curr.point.y; // set current coords.
    float x_next = map_goal.point.x, y_next = map_goal.point.y; // set coords. of next waypoint
    float delta_x = x_next - x_curr, delta_y = y_next - y_curr;   // change in coords.
    float yaw_curr = 0, pitch_curr = 0, roll_curr = 0;
    yaw_curr = atan2(delta_y, delta_x);

    // Specify quaternions
    rot_euler.setEulerYPR(yaw_curr, pitch_curr, roll_curr);
    rot_euler.getRotation(rot_quat);

    goal.target_pose.pose.orientation.x = rot_quat.getX();
    goal.target_pose.pose.orientation.y = rot_quat.getY();
    goal.target_pose.pose.orientation.z = rot_quat.getZ();
    goal.target_pose.pose.orientation.w = rot_quat.getW();

    return goal;
}


move_base_msgs::MoveBaseGoal half_rotation()
{
    move_base_msgs::MoveBaseGoal goal;
    tf::Matrix3x3 rot_euler;
    tf::Quaternion rot_quat;
    //Specify what frame we want the goal to be published in
    goal.target_pose.header.frame_id = "base_link";
    goal.target_pose.header.stamp = ros::Time::now();
    float yaw_curr = 0, pitch_curr = 0, roll_curr = 0;
    yaw_curr = 3.14;

    // Specify quaternions
    rot_euler.setEulerYPR(yaw_curr, pitch_curr, roll_curr);
    rot_euler.getRotation(rot_quat);
    // Specify x and y, w goal
    goal.target_pose.pose.position.x = 0.0; //specify x goal
    goal.target_pose.pose.position.y = 0.0; //specify y goal
    // goal.target_pose.pose.orientation.z = 1.0;
    // goal.target_pose.pose.orientation.w = 0.0;
    goal.target_pose.pose.orientation.x = rot_quat.getX();
    goal.target_pose.pose.orientation.y = rot_quat.getY();
    goal.target_pose.pose.orientation.z = rot_quat.getZ();
    goal.target_pose.pose.orientation.w = rot_quat.getW();
    return goal;
}

move_base_msgs::MoveBaseGoal simpleGoal(tf::StampedTransform transform)
{
    move_base_msgs::MoveBaseGoal goal;
    tf::Matrix3x3 rot_euler;
    tf::Quaternion rot_quat;  
    float X0,Y0,theta,X,Y,yaw_goal,x=1,y=0;

    rot_quat = transform.getRotation();
    X0 = transform.getOrigin().x();
    Y0 = transform.getOrigin().y();
    theta = rot_quat.getAngle();
    
    ROS_INFO("Origin: X0 = %f,Y0 = %f\n",X0,Y0);
    ROS_INFO("Rotation: z = %f, w = %f\n",rot_quat.getZ(),rot_quat.getW());
    ROS_INFO("Base <-- Odom rot angle: theta = %f\n",theta);

    X = x*cos(theta) - y*sin(theta) + X0;
    Y = y*cos(theta) + x*sin(theta) + Y0;
    yaw_goal = atan2(Y-Y0,X-X0);

    ROS_INFO("GOAL: X = %f,Y = %f\n",X,Y);
    rot_euler.setEulerYPR(yaw_goal, 0, 0);
    rot_euler.getRotation(rot_quat);
    //Specify what frame we want the goal to be published in
    goal.target_pose.header.frame_id = "odom";
    goal.target_pose.header.stamp = ros::Time::now();

    // Specify x and y goal
    goal.target_pose.pose.position.x = X; //specify x goal
    goal.target_pose.pose.position.y = Y; //specify y goal
    goal.target_pose.pose.orientation.x = rot_quat.getX();
    goal.target_pose.pose.orientation.y = rot_quat.getY();
    goal.target_pose.pose.orientation.z = rot_quat.getZ();
    goal.target_pose.pose.orientation.w = rot_quat.getW();
/*    goal.target_pose.pose.orientation.w = 1.0;*/
    
    return goal;
}