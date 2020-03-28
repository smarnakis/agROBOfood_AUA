#!/usr/bin/env python
import rospy
from std_msgs.msg import String,Bool
from sensor_msgs.msg import LaserScan
from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
import math
import numpy as np

previous_call = 0.0

def deg_rad(deg):
    return (deg*math.pi)/180

def readLaserData(data,e_pub):

    global previous_call

    time_stamp = data.header.stamp
    min_angle = data.angle_min
    max_angle = data.angle_max
    angle_inc = data.angle_increment
    off = (max_angle - min_angle)/angle_inc

    left = int((deg_rad(-55) - min_angle)/angle_inc)
    right = int((deg_rad(55) - min_angle)/angle_inc)
    front_left = int((deg_rad(-30) - min_angle)/angle_inc)
    front_right = int((deg_rad(30) - min_angle)/angle_inc)    
    num_particles = len(data.ranges)

    current_call = math.ceil(data.header.stamp.to_sec())
    if previous_call == 0.0:
        previous_call = math.ceil(data.header.stamp.to_sec())
    if current_call > previous_call:
        previous_call = current_call
        if any(value<20.0 for value in data.ranges[:left]):
            min_range = np.min(data.ranges[:left])
            if min_range < 2:
                e_pub.publish(True)
                rospy.logwarn("Object at left closer than 1m!!!")
            else:
                rospy.loginfo("Object at left!")
        else:
            rospy.loginfo("Left Clear.")
        if any(value<20.0 for value in data.ranges[left:front_left]):
            min_range = np.min(data.ranges[left:front_left])
            if min_range < 2:
                e_pub.publish(True)
                rospy.logwarn("Object at front left closer than 1m!!!")
            else:
                rospy.loginfo("Object at front left!")
        else:
            rospy.loginfo("Front left Clear.")
        if any(value<20.0 for value in data.ranges[front_left:front_right]):
            min_range = np.min(data.ranges[front_left:front_right])
            if min_range < 2:
                rospy.logwarn("Object forward closer than 1m!!!")
                e_pub.publish(True)
            else:
                rospy.loginfo("Object forward!")
                #e_pub.publish(False)
        else:
            e_pub.publish(False)
            rospy.loginfo("Forward Clear.")
        if any(value<20.0 for value in data.ranges[front_right:right]):
            min_range = np.min(data.ranges[front_right:right])
            if min_range < 2:
                e_pub.publish(True)
                rospy.logwarn("Object at front right closer than 1m!!!")
            else:
                rospy.loginfo("Object at front right!")
        else:
            rospy.loginfo("Front right Clear.")
        if any(value<20.0 for value in data.ranges[right:]):
            min_range = np.min(data.ranges[right:])
            if min_range < 2:
                e_pub.publish(True)
                rospy.logwarn("Object at right closer than 1m!!!")
            else:
                rospy.loginfo("Object at right!")
        else:
            rospy.loginfo("Right Clear.")
    #rate = rospy.Rate(1)
    #rate.sleep()
    
    

if __name__ == '__main__':

    rospy.init_node('protector', anonymous=True)

    e_pub = rospy.Publisher('emergency_stop', Bool, queue_size=10)

    rospy.Subscriber("scan", LaserScan, readLaserData,e_pub)

    ## For visualization purposes ==> to be transfered in new script
    """
    theta = 0
    pub = rospy.Publisher('visualization_marker', Marker,queue_size=10)
    while not rospy.is_shutdown():
        marker = Marker()
        marker.header.frame_id = "base_laser"
        marker.type = marker.ARROW
        marker.action = marker.ADD
        marker.scale.x = 1.0
        marker.scale.y = 0.01
        marker.scale.z = 0.01
        marker.color.a = 1.0
        marker.pose.orientation.z = math.sin(theta/2)
        marker.pose.orientation.w = math.cos(theta/2)
        marker.pose.position.x = 0.0
        marker.pose.position.y = 0.0
        marker.pose.position.z = 0.0

        pub.publish(marker)
    # spin() simply keeps python from exiting until this node is stopped
    """
    rospy.spin()