#include "ros/ros.h"
#include "gazebo_msgs/ModelState.h"
#include "gazebo_msgs/SetModelState.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "gazeboChangeStart");

    ros::NodeHandle nh;
    ros::ServiceClient client = nh.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");

    gazebo_msgs::SetModelState srv;
    srv.request.model_state.model_name = "turtlebot3_burger";
    srv.request.model_state.pose.position.x = 2;
    srv.request.model_state.pose.position.y = 1;
    srv.request.model_state.pose.position.z = 0;
    srv.request.model_state.pose.orientation.x = 0;
    srv.request.model_state.pose.orientation.y = 0;
    srv.request.model_state.pose.orientation.z = 0;
    srv.request.model_state.pose.orientation.w = 1;
    client.call(srv);

    return 0;
}
