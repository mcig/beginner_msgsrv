#!/usr/bin/env python3

from __future__ import print_function

from beginner_msgsrv.srv import CalcSteeringAngle, CalcSteeringAngleResponse
import rospy
from math import atan2

def handleCalculate(req):
    x1, x2, y1, y2 = req.x1, req.x2, req.y1, req.y2
    print(f"Returning the angle between [{x1}, {x2}, {y1}, {y2}]")
    angle = atan2(y2 - y1, x2 - x1)
    print(f"Angle: {angle}")
    return CalcSteeringAngleResponse(angle)

def calculate_steering_angle():
    rospy.init_node('calculate_steering_angle')
    
    s = rospy.Service('calculate_steering_angle', CalcSteeringAngle, handleCalculate)
    print("Ready to calculate the steering angle.")

    #rospy.spin() keeps your code from exiting until the service is shutdown.
    rospy.spin()

if __name__ == "__main__":
    calculate_steering_angle()