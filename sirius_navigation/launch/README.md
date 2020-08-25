## sirius_navigation pkg launch files ##

This folder contains the launch files which compose the navigation process of Sirius.

1. **navigation folder**
	
	In this folder there are all the launch files responsible for firing up the nodes of the [ros navigation stack](http://wiki.ros.org/navigation) 
- [move_base](http://wiki.ros.org/move_base?distro=kinetic)
- [gmapping](http://wiki.ros.org/gmapping)
- [amcl](http://wiki.ros.org/amcl?distro=kinetic)

2. **Sirius Outdoor Tasks**
	
	In order for Sirius to navigate autonomously in the field, there are 3 launch files in this folder.

- **outdoor_nav.launch**: This launch file is the core of outdoor navigation. It enables the map_localization which is based on an absolute reference frame (map). It offers to the user 3 modes:
	- no_static_map:=true: The robot navigates without a static map. It uses gps as the global referce frame along with a local and global costmap.
	- no_static_map:=false. The robot navigates using a static map.
		- slam:=true: The robot navigates using a static map that it creates on the spot using Simultaneous Localisation and Mapping. 
		- slam:=false: The robot navigates using a static map that is loaded from the map server and is created by a previous mapping process.

- **collect_gps_waypoints.launch**: This launch file is responsible for collecting gps waypoints (latitude and longitude). Its main purpose is to enable the user to collect the waypoints in real-time using by sending *Sirius* to the desired locations via joystick and then pressing the collection button on the joystick. All the parameters involved to the node that is invoked by this launch file can be found under *config/collector.yaml*.

- **planning.launch**: This launch file is responsible for firing up one of the 2 custom gps planners of *Sirius* to perform waypoint navigation. The planners are configured by the file *config/plan_config.yaml*.