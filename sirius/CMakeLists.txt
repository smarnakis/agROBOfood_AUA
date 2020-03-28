cmake_minimum_required(VERSION 2.8.3)
project(sirius)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  husky_gazebo
  roscpp
  rospy
  std_msgs
  actionlib
  actionlib_msgs
  move_base_msgs
)


generate_messages(DEPENDENCIES std_msgs actionlib_msgs)

catkin_package(		
)

include_directories(
	include 
	${catkin_INCLUDE_DIRS}
)


add_executable(simple_planer src/simple_planer.cpp)
add_executable(emergancy_handler src/emergency_node.cpp)

target_link_libraries(simple_planer ${catkin_LIBRARIES})
target_link_libraries(emergancy_handler ${catkin_LIBRARIES})