cmake_minimum_required(VERSION 3.0.2)
project(sirius_navigation)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roslib
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

######## EXACUTABLES #########
## LEGACY EXECUTABLES
add_executable(simple_planer src/legacy/simple_planer.cpp src/functions.cpp)
add_executable(half_rotation src/legacy/half_rotation.cpp src/functions.cpp)

## INDOOR TAKS
add_executable(map_coords_planner src/map_coords_planner.cpp src/functions.cpp)
add_executable(collect_waypoints src/collect_waypoints.cpp)

## OUTDOOR TASKS
add_executable(collect_gps_waypoints src/collect_gps_waypoints.cpp)
add_executable(gps_planner src/gps_planer.cpp src/functions.cpp)
add_executable(gps_planner_2 src/gps_planner_2.cpp src/functions.cpp)
add_executable(gps_planner_3 src/gps_planner_3.cpp src/functions.cpp)

## UTILS 
add_executable(keyboard src/keyboard.cpp src/functions.cpp)


######## LINKING LIBRARIES #########
target_link_libraries(simple_planer ${catkin_LIBRARIES})
target_link_libraries(half_rotation ${catkin_LIBRARIES})

target_link_libraries(map_coords_planner ${catkin_LIBRARIES})
target_link_libraries(collect_waypoints ${catkin_LIBRARIES})

target_link_libraries(collect_gps_waypoints ${catkin_LIBRARIES})
target_link_libraries(gps_planner ${catkin_LIBRARIES})
target_link_libraries(gps_planner_2 ${catkin_LIBRARIES})
target_link_libraries(gps_planner_3 ${catkin_LIBRARIES})

target_link_libraries(keyboard ${catkin_LIBRARIES})
