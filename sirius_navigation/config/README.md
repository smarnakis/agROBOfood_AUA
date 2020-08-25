## sirius_navigation pkg configuration files ##

This folder contains the configuration files for the following jobs:

- THE OUTDOOR NAVIGATION PROCESS
	- costmap_common.yaml
	- costmap_global_laser.yaml
	- costmap_global_static.yaml
	- costmap_local.yaml
	- planner.yaml
- THE GPS WAYPOINT COLLECTOR
	- collector.yaml
- THE CUSTOM GPS PLANNERS
	- plan_config.yaml

In the most important paramareters to be tuned are the following:

1. **MOVE BASE PARAMETERS.**

    The exact utility of each parameter can be found under:
    
    [move_base](http://wiki.ros.org/move_base)
    [costmap_2d](http://wiki.ros.org/costmap_2d?distro=kinetic)
    
    while in general:
- costmap_common.yaml: Contains params used both by the local and the global planner
- planner.yaml: Contains params configuring the selected movebase planners.

2. **COLLECTOR PARAMETERS**

    This file is loaded by the executable **collect_gps_waypoints**, and it contains the following parameters:

- keyboard_option: 0 --> joystick collection, 1 --> keyboard collection. Current option ==> joystick.
- collect_button_num: An integer coresponding to the joystick button desired for collection. The correlation between the int values and the joystick bottons of the logitech joystick can be found at [joystick](http://wiki.ros.org/joy). Current option ==> B
- end_button_num1,end_button_num2: The two joystick buttons that are pushed conbined to end the waypoint collection. Current options ==> LB,RB
- key_collect_button: Keyboard collection button. Current option ==> 1
- key_end_button: Keyboard button to end collection. Current option ==> 2
- coordinates_file: local path in **sirius_navigation** pkg to save the coordinates file. This param is used only by the indoor collector.
- gps_coordinates_file: local path in **sirius_navigation** pkg to save the gps coordinated file. This param is used only by the outdoor collector. 

3. **CUSTOM PLANNERS PARAMETES**

	Finally, the gps planner are configired via the plan_config.yaml file.

- trigger_button1,trigger_button2: These params are used only by **gps_planner_3**. They are used to trigger the motion from to the next gps waypoint. Current options ==> LB,RB.
- coordinates_file: The local path to the file from which the indoor planner loads the waypoints. 
- gps_coordinates_file: The local path to the file from which the outdoor gps planners load the gps waypoints.