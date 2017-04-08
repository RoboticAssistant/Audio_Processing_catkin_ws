#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include "tts.h"
#include "stt.h"

#include "ros/ros.h"
#include <iostream>

using namespace std;

pthread_t t1,t2,t3,t4;

int argc1;
char** argv1;
char *transmit, receive[1000];

extern cmd_ln_t *config;
extern ps_decoder_t *ps;
extern std_msgs::String publish;
extern std_msgs::String subscribe;
extern string str;
string tts_str = "";

void  *STT(void *arg)
{
	ad_rec_t *ad;
	int16 adbuf[2048];
    	uint8 utt_started, in_speech;
    	int32 k;
	//publish.data = "";

/*******Initialize*******/	
    	utt_started = FALSE;
	stt_init();

	//if(!(ros::ok()))
        //	E_FATAL("ROS master not running\n");	

	if ((ad = ad_open_dev("plughw:DEV=0,CARD=1", (int) cmd_ln_float32_r(config,"-samprate"))) == NULL)
	  	E_FATAL("Failed to open audio device\n");

	if (ad_start_rec(ad) < 0)
        	E_FATAL("Failed to start recording\n");

    	if (ps_start_utt(ps) < 0)
        	E_FATAL("Failed to start utterance\n");	

/*******Run*******/
    	E_INFO("Ready....\n");
	while(1)
    	{		
		recognize(ad, adbuf, &utt_started, &in_speech, &k);
     	}

    	ad_close(ad);

    	ps_free(ps);
    	cmd_ln_free_r(config);

}

void  *TTS(void *arg)
{
/*******Initialize*******/

/*******Run*******/
	while(1)
	{
		if(tts_str != "")
		{
			make_audio((char*)tts_str.c_str());
			play_audio();
			tts_str = "";	
		}

	}
}

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  if(*msg->data.c_str())
  {
	cout<<msg->data.c_str()<<endl;
	string temp = msg->data;

	if(!temp.compare("BOOT"))
		tts_str = "Hello everyone! System started.";

  }
}

void faceCallback(const std_msgs::String::ConstPtr& msg)
{
  if(*msg->data.c_str())
  {
	cout<<msg->data.c_str()<<endl;

	tts_str = "Number of faces detected " + msg->data;
  }
}

void  *PUBSUB(void *arg)
{
	subscribe.data = "";

	ros::init(argc1, argv1, "audio_module");
	
	ros::NodeHandle n;	
	ros::Publisher direction = n.advertise<std_msgs::String>("A_D_direction", 1000);	

	ros::Publisher rotation = n.advertise<std_msgs::String>("A_D_rotation", 1000);	
	
	ros::Rate loop_rate(10);

  	ros::Subscriber sub = n.subscribe("status", 10, chatterCallback);

  	ros::Subscriber sub_faces = n.subscribe("face_information", 10, faceCallback);
	
	while (ros::ok())
	{

		if(!str.compare("FORWARD") || !str.compare("BACKWARD") || !str.compare("STOP"))
		{	
			publish.data = str;
			direction.publish(publish);
			cout<<str<<endl<<endl;
		}
			
		if(!str.compare("LEFT") || !str.compare("RIGHT"))
		{
			/*if(str.compare("MOVE LEFT"))	
				publish.data = "LEFT";

			if(str.compare("MOVE RIGHT"))	
				publish.data = "RIGHT";*/

			publish.data = str;

			rotation.publish(publish);
			cout<<str<<endl<<endl;
		}


		//tts_str = str;	//this line will replay the audio command heard
		str = "";

		ros::spinOnce();
		
		loop_rate.sleep();
	}
}

void  *PLOT(void *arg)
{
	while(1)
	{
		string speak;
		getline(cin,speak);
		if(speak != "")
		{
			make_audio((char*)speak.c_str());
			play_audio();
			speak = "";	
		}
		
	}

}

int main(int argc, char **argv)
{
	argc1 = argc;
	argv1 = argv;

	char command[1000];
	sprintf(command,"bash %sRecord_from_DMIC.sh",APP_DIR);	
 	system(command);

	sprintf(command,"bash %sPlayback_to_Headset.sh",APP_DIR);	
 	system(command);

	pthread_create (&t1, NULL, &STT, NULL);
	pthread_create (&t2, NULL, &TTS, NULL);
	pthread_create (&t3, NULL, &PLOT, NULL);
	pthread_create (&t4, NULL, &PUBSUB, NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);

	return 0;
}