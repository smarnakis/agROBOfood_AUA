#!/usr/bin/env python

import rospy
from std_msgs.msg import String,Bool
from sensor_msgs.msg import LaserScan
from sensor_msgs.msg import NavSatFix
from nav_msgs.msg import Odometry
from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
import matplotlib.pyplot as plt
import math
import numpy as np
import rospkg
import os
import sys

lat,lon,alt = [],[],[]
latf,lonf,altf = [],[],[]
x,y = [],[]

def gather_map_keypoints(odom_msg):
	x.append(odom_msg.pose.pose.position.x)
	y.append(odom_msg.pose.pose.position.y)

def gather_fix_keypoints(fix_msg):
	# global last_call
	# time_call = rospy.get_time()
	# print("time_call:",time_call)
	lati = fix_msg.latitude
	longi = fix_msg.longitude
	alti	=	fix_msg.altitude
	if not (math.isnan(lati)): 
		latf.append(lati)
		lonf.append(longi)
		altf.append(alti)

def gather_gps_keypoints(fix_msg):
	# global last_call
	# time_call = rospy.get_time()
	# print("time_call:",time_call)
	lati = fix_msg.latitude
	longi = fix_msg.longitude
	alti	=	fix_msg.altitude
	if not (math.isnan(lati)): 
		lat.append(lati)
		lon.append(longi)
		alt.append(alti)

def main():
	rospy.init_node('GPS_vis', anonymous=True)
	# time_curr = rospy.get_time()
	global last_call
	sub_gps = rospy.Subscriber("/gps/filtered", NavSatFix, gather_gps_keypoints)
	sub_fix = rospy.Subscriber("/navsat/fix", NavSatFix, gather_fix_keypoints)
	sub_map = rospy.Subscriber("/odometry/filtered_map", Odometry, gather_map_keypoints)

	while(not rospy.is_shutdown()):
		rospy.spin()
	if rospy.is_shutdown():
		rospack = rospkg.RosPack()
		PKG_DIR = rospack.get_path('sirius_navigation')
		LOCAL_DIR = "txt_files"
		FILE_NAME = sys.argv[1]
		ABS_PATH = os.path.join(PKG_DIR,LOCAL_DIR,FILE_NAME)
		print(ABS_PATH)
		f = open(ABS_PATH,"wt")
		f.write(str(latf))
		f.write("\n")
		f.write(str(lonf))
		f.write("\n")
		f.write(str(altf))
		f.write("\n")
		f.write(str(lat))
		f.write("\n")
		f.write(str(lon))
		f.write("\n")
		f.write(str(alt))
		f.write("\n")
		f.write(str(x))
		f.write("\n")
		f.write(str(y))
		f.close()

if __name__ == '__main__':
	try:
		main()
	except rospy.ROSInterruptException:
		pass