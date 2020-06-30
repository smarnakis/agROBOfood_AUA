#!/usr/bin/env python

import rospy
from std_msgs.msg import String,Bool
from sensor_msgs.msg import LaserScan
from sensor_msgs.msg import NavSatFix
from nav_msgs.msg import Odometry
from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray

import math
import numpy as np
import rospkg
import os
import sys

lat,lon,alt,rtk = [],[],[],[]
latf,lonf,altf,rtkf = [],[],[],[]
latf_r,lonf_r,altf_r,rtkf_r = [],[],[],[]
x,y = [],[]
time_fix,time_fix_r,time_gps,time_map = [],[],[],[]

def gather_map_keypoints(odom_msg):
	time_map.append(odom_msg.header.stamp.to_sec())
	x.append(odom_msg.pose.pose.position.x)
	y.append(odom_msg.pose.pose.position.y)

def gather_fix_keypoints(fix_msg):
	# global last_call
	# time_call = rospy.get_time()
	# print("time_call:",time_call)
	lati = fix_msg.latitude
	longi = fix_msg.longitude
	alti	=	fix_msg.altitude
	rtki = fix_msg.status.status
	now = fix_msg.header.stamp.to_sec()
	if not (math.isnan(lati)):
		time_fix.append(now)
		latf.append(lati)
		lonf.append(longi)
		altf.append(alti)
		rtkf.append(rtki)

def gather_fix_raw_keypoints(fix_msg):
	# global last_call
	# time_call = rospy.get_time()
	# print("time_call:",time_call)
	lati = fix_msg.latitude
	longi = fix_msg.longitude
	alti	=	fix_msg.altitude
	rtki = fix_msg.status.status
	now = fix_msg.header.stamp.to_sec()
	if not (math.isnan(lati)):
		time_fix_r.append(now)
		latf_r.append(lati)
		lonf_r.append(longi)
		altf_r.append(alti)
		rtkf_r.append(rtki)

def gather_gps_keypoints(fix_msg):
	# global last_call
	# time_call = rospy.get_time()
	# print("time_call:",time_call)
	lati = fix_msg.latitude
	longi = fix_msg.longitude
	alti	=	fix_msg.altitude
	rtki = fix_msg.status.status
	now = fix_msg.header.stamp.to_sec()
	if not (math.isnan(lati)):
		time_gps.append(now)
		lat.append(lati)
		lon.append(longi)
		alt.append(alti)
		rtk.append(rtki)

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
		if len(sys.argv) > 1:
			FILE_NAME = sys.argv[1]
		else:
			FILE_NAME = "rtk_evaluation.txt"
		ABS_PATH = os.path.join(PKG_DIR,LOCAL_DIR,FILE_NAME)
		print(ABS_PATH)
		print(sys.argv)
		f = open(ABS_PATH,"wt")
		f.write("Raw fix keypoints")
		f.write("\n")
		f.write(str(time_fix))
		f.write("\n")
		f.write(str(latf))
		f.write("\n")
		f.write(str(lonf))
		f.write("\n")
		f.write(str(altf))
		f.write("\n")
		f.write(str(rtkf))
		f.write("\n")
		f.write("Filtered gps keypoints")
		f.write("\n")
		f.write(str(time_gps))
		f.write("\n")
		f.write(str(lat))
		f.write("\n")
		f.write(str(lon))
		f.write("\n")
		f.write(str(alt))
		f.write("\n")
		f.write(str(rtk))
		f.write("\n")
		f.write("Map keypoints")
		f.write("\n")
		f.write(str(time_map))
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
