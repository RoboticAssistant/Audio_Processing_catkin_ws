#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>

using namespace std;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  //if(*msg->data.c_str())
	cout<<msg->data.c_str()<<endl;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "subscribe");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("audio", 1000, chatterCallback);

  ros::spin();

  return 0;
}