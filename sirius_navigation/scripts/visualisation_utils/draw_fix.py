#!/usr/bin/env python

import matplotlib.pyplot as plt
import math
import numpy as np
import os
import ast

def main():

	DIR = "/home/smarn/ros/aua_ws/src/agROBOfood_AUA/sirius_navigation/txt_files"
	FILE_NAME = "moving_slam.txt"
	ABS_PATH = os.path.join(DIR,FILE_NAME)
	# ABS_PATH1 = os.path.join(PKG_DIR,LOCAL_DIR,FILE_NAME1)
	print(ABS_PATH)
	f = open(ABS_PATH,"rt")
	lines = f.readlines()
	time_raw_fix = ast.literal_eval(lines[1])
	lat_fix = ast.literal_eval(lines[2])
	lon_fix = ast.literal_eval(lines[3])
	time_filt_fix = ast.literal_eval(lines[7])
	lat_filt = ast.literal_eval(lines[8])
	lon_filt = ast.literal_eval(lines[9])
	time_map = ast.literal_eval(lines[13])
	x_map = ast.literal_eval(lines[14])
	y_map = ast.literal_eval(lines[15])
	# alt = ast.literal_eval(lines[5])
	f.close()

	time_map = [i/60 for i in time_map]
	time_filt_fix = [i/60 for i in time_filt_fix]
	time_raw_fix = [i/60 for i in time_raw_fix]

	fig,axs1 = plt.subplots(1,2)
	axs1[0].grid('on')
	axs1[1].grid('on')

	axs1[0].set_xlabel("X (m)")
	axs1[0].set_ylabel("Y (m)")
	axs1[1].set_xlabel("Latitude")
	axs1[1].set_ylabel("Longitude")
	axs1[0].plot(x_map,y_map)
	axs1[1].plot(lon_fix,lat_fix)
	# plt.show()

	fig,axs = plt.subplots(2,2)
	axs[0,0].grid('on')
	axs[0,1].grid('on')
	axs[1,0].grid('on')
	axs[1,1].grid('on')

	axs[0,0].set_ylabel("X (m)")
	axs[0,1].set_ylabel("Y (m)")
	axs[1,0].set_ylabel("Latitude")
	axs[1,1].set_ylabel("Longitude")
	axs[0,0].set_xlabel("time (sec)")
	axs[0,1].set_xlabel("time (sec)")
	axs[1,0].set_xlabel("time (sec)")
	axs[1,1].set_xlabel("time (sec)")
	axs[0,0].plot(time_map,x_map)
	axs[0,1].plot(time_map,y_map)
	axs[1,1].plot(time_raw_fix,lat_fix)
	axs[1,1].plot(time_filt_fix,lat_filt)
	axs[1,0].plot(time_raw_fix,lon_fix)
	axs[1,0].plot(time_filt_fix,lon_filt)
	plt.show()



if __name__ == '__main__':
	main()