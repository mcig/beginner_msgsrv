#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <geometry_msgs/Pose2D.h>
#include <math.h>

ros::Publisher pub;
geometry_msgs::Twist msg;
geometry_msgs::Pose2D current_pose;
void coolRotate(ros::NodeHandle, double);

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
    ROS_INFO("Current Pose: x=%f, y=%f, theta=%f", current_pose.x, current_pose.y, current_pose.theta);
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
    ROS_INFO("Moved 2m forward");
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "odomnode");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/odom", 1000, odomCallback);
    coolRotate(n, 90);
    coolRotate(n, 60);
    coolRotate(n, 120);
    ros::spin();

    return 0;
}

void coolRotate(ros::NodeHandle n, double degrees)
{
    pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
    ros::Rate loop_rate(10);

    msg.linear.x = 0;
    msg.angular.z = 0.2;

    double targetrad = degrees * M_PI / 180.0;
    double start_theta = current_pose.theta;
    double c = 0.5;

    while (ros::ok())
    {
        std::cout << "Target: " << msg.angular.z << std::endl;
        msg.linear.x = 0;
        msg.angular.z = c * (targetrad - current_pose.theta);

        // handle negative radians using M_PI
        if (msg.angular.z < 0)
        {
            // calculate using pi
            msg.angular.z = c * (targetrad - current_pose.theta - 2 * M_PI);
            std::cout << "CALC: " << msg.angular.z << std::endl;
        }

        if (targetrad - start_theta > 0 && msg.angular.z < 0.008)
            break;
        else if (targetrad - start_theta < 0 && msg.angular.z > 0.008)
            break;

        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    msg.linear.x = 0;
    msg.angular.z = 0;
    pub.publish(msg);
    ROS_INFO("Rotated %f degrees", degrees);
}