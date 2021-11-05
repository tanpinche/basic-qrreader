#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include "espdrone_msgs/FullState.h"
#include "espdrone_msgs/BatteryStatus.h"
#include <std_msgs/Duration.h>
#include <cstdlib>
 
#include "espdrone_msgs/GoTo.h"
#include "espdrone_msgs/Takeoff.h"
#include "espdrone_msgs/Hover.h"
 
int main(int argc, char **argv)
{
 ros::init(argc, argv, "basicclient");
 ros::NodeHandle n;
 ros::ServiceClient client = n.serviceClient<espdrone_msgs::GoTo>("/espdrone178/go_to");
 ros::ServiceClient client2 = n.serviceClient<espdrone_msgs::Takeoff>("/espdrone178/Takeoff");
 ros::Publisher hover_pub = n.advertise<espdrone_msgs::Hover>("/espdrone178/cmd_hover", 1000);    
ros::Rate loop_rate(10);

 ros::Duration five_seconds(5.0);
 espdrone_msgs::GoTo waypoint;
 espdrone_msgs::Takeoff takeoff;
 waypoint.request.duration = five_seconds;
 waypoint.request.relative = false;
 waypoint.request.goal.x=0.09;
 waypoint.request.goal.y=0.07;
 waypoint.request.goal.z= 1;
 waypoint.request.yaw = 0.5;
 takeoff.request.height = 1;
 takeoff.request.duration = five_seconds;



 if(client2.call(takeoff)){
ROS_INFO("taking off");

 }

 else {
     ROS_INFO("failed to take off");

 }

 ros::Duration(5.0).sleep(); 

 if (client.call(waypoint))
 {
 ROS_INFO("moving to point");



 
//  client.call(waypoint);


 }
 else
 {
 ROS_ERROR("Failed to call service");
 return 1;
 }

 ros::Duration(4.0).sleep(); 
 int count =0;
espdrone_msgs::Hover hover;
hover.vx = 0;
hover.vy = 0;
hover.yawrate =0;
hover.zDistance = 1;
ROS_INFO("hovering");
while(count<100){
        hover_pub.publish(hover);
        count ++;
        ros::Duration(0.1).sleep();
    }

ROS_INFO("hover end");
 ROS_INFO("calling next waypoint");

 waypoint.request.duration = five_seconds;
 waypoint.request.relative = false;
 waypoint.request.goal.x=0.09;
 waypoint.request.goal.y=0.07;
 waypoint.request.goal.z= 1.5;
 waypoint.request.yaw = 0;

 if (client.call(waypoint))
 {
 ROS_INFO("moving to next point");

 }
 else
 {
 ROS_ERROR("Failed to call service");
 return 1;
 }


ros::Duration(5.0).sleep(); 
 ROS_INFO("5seconds passed, calling next waypoint");

 waypoint.request.duration = five_seconds;
 waypoint.request.relative = false;
 waypoint.request.goal.x=0.09;
 waypoint.request.goal.y=0.07;
 waypoint.request.goal.z= 1;
 waypoint.request.yaw = 0;

 if (client.call(waypoint))
 {
 ROS_INFO("moving to last point");

 }
 else
 {
 ROS_ERROR("Failed to call service");
 return 1;
 }

// ros::Duration(5.0).sleep(); 
//  waypoint.request.duration = five_seconds;
//  waypoint.request.relative = false;
//  waypoint.request.goal.x=0;
//  waypoint.request.goal.y=0.5;
//  waypoint.request.goal.z= 0.5;
//  waypoint.request.yaw = 0;

 
//  client.call(waypoint);

 
 return 0;
}