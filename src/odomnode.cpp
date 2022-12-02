#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <geometry_msgs/Pose2D.h>
#include <math.h>

ros::Publisher pub;
geometry_msgs::Twist msg;
geometry_msgs::Pose2D current_pose;

void odomCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    current_pose.x = msg->pose.pose.position.x;
    current_pose.y = msg->pose.pose.position.y;

    tf::Quaternion q(
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);

    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    current_pose.theta = yaw;
}

void move(ros::NodeHandle n)
{
    pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
    ros::Rate loop_rate(10);

    while (ros::ok() && current_pose.x < 2.0)
    {
        msg.linear.x = 0.1;
        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    msg.linear.x = 0;
    msg.angular.z = 0;
    pub.publish(msg);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "odomnode");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/odom", 1000, odomCallback);
    move(n);
    ros::spin();

    return 0;
}