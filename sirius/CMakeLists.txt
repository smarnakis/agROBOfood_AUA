cmake_minimum_required(VERSION 2.8.3)
project(sirius)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  actionlib
  actionlib_msgs
  move_base_msgs
  sensor_msgs
  geometry_msgs
  robot_localization
  tf
)


generate_messages(DEPENDENCIES 
	std_msgs 
  sensor_msgs
	actionlib_msgs 
	geometry_msgs
)

catkin_package(		
)

include_directories(
	include 
	${catkin_INCLUDE_DIRS}
)


add_executable(safety src/safety.cpp)


target_link_libraries(safety ${catkin_LIBRARIES})