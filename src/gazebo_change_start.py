#!/usr/bin/env python3

import rospy
from gazebo_msgs.srv import SetModelState
from gazebo_msgs.msg import ModelState

rospy.init_node('change_start', anonymous=True)
rospy.wait_for_service('/gazebo/set_model_state')

try:
    change_start = rospy.ServiceProxy('/gazebo/set_model_state', SetModelState)
    state = ModelState()
    state.model_name = 'turtlebot3_burger'
    state.pose.position.x = 1
    state.pose.position.y = 1
    state.pose.position.z = 0
    state.pose.orientation.x = 0
    state.pose.orientation.y = 0
    state.pose.orientation.z = 0
    state.pose.orientation.w = 1
    resp = change_start(state)
    print("Moved robot to x=1, y=1")
except rospy.ServiceException as e:
    print("Service call failed: %s"%e)

    