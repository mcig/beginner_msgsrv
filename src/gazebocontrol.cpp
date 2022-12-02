#include <ros/ros.h>
#include <std_srvs/Empty.h>
#include "std_msgs/String.h"
#include "gazebo_msgs/SetModelState.h"

using namespace std;

ros::Publisher velocity_pub;
geometry_msgs::Twist msg;

void move_distance(ros::NodeHandle nh, double distance)
{
    ros::Rate loop_rate(10);
    double current_dist = 0;
    ros::Time t0 = ros::Time::now();
    while (current_dist < distance)
    {
        msg.linear.x = 0.1;
        velocity_pub.publish(msg);
        ros::Time t1 = ros::Time::now();
        current_dist = msg.linear.x * (t1 - t0).toSec();
        loop_rate.sleep();
    }
    msg.linear.x = 0;
    msg.angular.z = 0;
    velocity_pub.publish(msg);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "gazebocontrol");
    ros::NodeHandle nh;
    velocity_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 100);

    ros::ServiceClient resetsim_client = nh.serviceClient<std_srvs::Empty>("/gazebo/reset_simulation");
    std_srvs::Empty srv;
    resetsim_client.call(srv);
    ROS_INFO("Reset simulation");

    move_distance(nh, 1);
    ROS_INFO("Moved 1m forward");

    ros::ServiceClient resetworld_client = nh.serviceClient<std_srvs::Empty>("/gazebo/reset_world");
    resetworld_client.call(srv);
    ROS_INFO("Reset world");

    return 0;
}