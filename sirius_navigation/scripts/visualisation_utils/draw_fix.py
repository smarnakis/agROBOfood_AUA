#!/usr/bin/env python

import rospy
from std_msgs.msg import String,Bool
from sensor_msgs.msg import LaserScan
from sensor_msgs.msg import NavSatFix
from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
import matplotlib.pyplot as plt
import math
import numpy as np
import rospkg
import os
import ast

def main():
	rospack = rospkg.RosPack()
	PKG_DIR = rospack.get_path('sirius_navigation')
	LOCAL_DIR = "txt_files"
	FILE_NAME = "navsat_fix_sim.txt"
	FILE_NAME1 = "navsat_fix.txt"	
	ABS_PATH = os.path.join(PKG_DIR,LOCAL_DIR,FILE_NAME)
	ABS_PATH1 = os.path.join(PKG_DIR,LOCAL_DIR,FILE_NAME1)
	print(ABS_PATH)
	f = open(ABS_PATH,"rt")
	lines = f.readlines()
	lat = ast.literal_eval(lines[3])
	lon = ast.literal_eval(lines[4])
	alt = ast.literal_eval(lines[2])
	f.close()
	# f = open(ABS_PATH1,"rt")
	# lines1 = f.readlines()
	# lat1 = ast.literal_eval(lines1[0])
	# lon1 = ast.literal_eval(lines1[1])
	# alt1 = ast.literal_eval(lines1[2])
	# f.close()
	lat_mean = np.mean(lat)
	lon_mean = np.mean(lon)
	# lat_mean1 = np.mean(lat1)
	# lon_mean1 = np.mean(lon1)
	# lat = lat - lat_mean
	# lon = lon - lon_mean
	# lat1 = lat1 - lat_mean1
	# lon1 = lon1 - lon_mean1
	print(lat_mean,lon_mean)
	fig = plt.figure()
	# plt.xlim([lat_mean-0.00001,lat_mean+0.00001])
	# plt.ylim([lon_mean-0.00001,lon_mean+0.00001])
	plt.plot(lat,lon)
	# plt.plot(lat1,lon1)
	plt.show()


if __name__ == '__main__':
	main()