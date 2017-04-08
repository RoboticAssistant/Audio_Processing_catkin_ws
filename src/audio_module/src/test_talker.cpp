#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <string>

using namespace std;

std_msgs::String publish;


int main(int argc, char **argv)
{
  	ros::init(argc, argv, "audio_module");

	ros::NodeHandle m;	
	ros::Publisher chatter_pub = m.advertise<std_msgs::String>("video", 1000);	
	ros::Rate loop_rate(10);

	while (ros::ok())
	{
		cout<<"Enter data.."<<endl;
		getline(cin,publish.data);
		cout<<"published.."<<endl;

		chatter_pub.publish(publish);		
		publish.data = "";
		
		ros::spinOnce();
		
		loop_rate.sleep();
	}

  	return 0;
}