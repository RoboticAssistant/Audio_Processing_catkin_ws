#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include "tts.h"
#include "stt.h"

#include "ros/ros.h"
#include <iostream>
#include <vector>

using namespace std;

pthread_t t1,t2,t3,t4;

int argc1;
char** argv1;
char *transmit, receive[1000];

extern cmd_ln_t *config;
extern ps_decoder_t *ps;
ad_rec_t *ad;

extern std_msgs::String publish;
extern std_msgs::String subscribe;
extern string str;
string tts_str = "";
string key_press = "";
bool keyword_accepted = false;
bool TTS_on = false;
bool get_location = false;
vector<string> speakerStack;

void INThandler(int sig)
{
	signal(sig,SIG_IGN);
    	ad_close(ad);
    	ps_free(ps);
    	cmd_ln_free_r(config);

	exit(0);	
}

void  *STT(void *arg)
{
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
		if(key_press == "1" && !TTS_on)
		{	
			recognize(ad, adbuf, &utt_started, &in_speech, &k);
			//printf("TTS_on = %d\n",TTS_on);
		}

		if(key_press == "0" || TTS_on)
		{
			for(int i=0;i<2048;i++)
				adbuf[i]=0;
			if(key_press == "0")
				key_press = "";
			utt_started = 0;
			in_speech = 0;
			k = 0;

		}

		signal(SIGINT, INThandler);		
     	}
}

void  *TTS(void *arg)
{
/*******Initialize*******/

/*******Run*******/
	while(1)
	{
		/*if(tts_str != "")
		{
			make_audio((char*)tts_str.c_str());
			play_audio();
			tts_str = "";	
		}*/
		if(speakerStack.size())
		{
			TTS_on = true;

			tts_str = speakerStack.back();			
			make_audio((char*)tts_str.c_str());
			play_audio();
			speakerStack.pop_back();
			sleep(1);

			TTS_on = false;
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
		speakerStack.push_back("System started.");
		//tts_str = "Hello everyone! System started.";

	if(!temp.compare("GREET"))
		speakerStack.push_back("Hello everyone!");
		//tts_str = "Hello everyone. Welcome to Computer Engineering.";	

  }
}

void faceCallback(const std_msgs::String::ConstPtr& msg)
{
  if(*msg->data.c_str())
  {
	cout<<msg->data.c_str()<<endl;
	speakerStack.push_back("Number of faces detected " + msg->data);
	//tts_str = "Number of faces detected " + msg->data;
  }
}

void locationCallback(const std_msgs::String::ConstPtr& msg)
{
  if(*msg->data.c_str() && get_location)
  {
	speakerStack.push_back("You are in " + msg->data);
	get_location = false;
  }
}

void objectCallback(const std_msgs::String::ConstPtr& msg)
{//cout<<" dsasdad#n"<<endl;
  if(*msg->data.c_str())
  {
	cout<<msg->data.c_str()<<endl;
	int object_count = atoi(msg->data.c_str());
	if(object_count)
	{
		char tempstr[500];
		sprintf(tempstr, "%d Objects found!",object_count);

		speakerStack.push_back((string)tempstr);

		int pos = msg->data.find_last_of(" ");
		
		int nearby_obj_count = 0;

		if(pos)
			nearby_obj_count = atoi(msg->data.substr(pos+1).c_str());

		if(nearby_obj_count)
		{
			sprintf(tempstr, "%d Nearby objects found!",nearby_obj_count);
			speakerStack.push_back((string)tempstr);	
		}


	}
  }
}

void  *PUBSUB(void *arg)
{
	subscribe.data = "";

	ros::init(argc1, argv1, "audio_module");
	
	ros::NodeHandle n;	
	ros::Publisher direction = n.advertise<std_msgs::String>("A_D_direction", 1000);	

	ros::Publisher rotation = n.advertise<std_msgs::String>("A_D_rotation", 1000);

	ros::Publisher destination = n.advertise<std_msgs::String>("destination_point", 1000);	
	
	ros::Rate loop_rate(10);

  	ros::Subscriber sub = n.subscribe("status", 10, chatterCallback);

  	ros::Subscriber sub_faces = n.subscribe("face_information", 10, faceCallback);
	
	ros::Subscriber sub_loc = n.subscribe("current_location", 10, locationCallback);

	ros::Subscriber sub_object = n.subscribe("object_information", 10, objectCallback);
	
	while (ros::ok())
	{
		int pos = str.find_last_of(" ");
		const char *substring = str.substr(pos+1).c_str();

		if(strstr(substring,"FORWARD") && keyword_accepted == true)
		{
			publish.data = "FORWARD";
			direction.publish(publish);
			cout<<str<<endl<<endl;
			speakerStack.push_back("Moving forward");
			keyword_accepted = false;
			str = "";
		}
		else if(strstr(substring,"BACKWARD") && keyword_accepted == true)
		{
			publish.data = "BACKWARD";
			direction.publish(publish);
			cout<<str<<endl<<endl;
			str = "";
			speakerStack.push_back("Moving backward");
			keyword_accepted = false;
		}
		else if(strstr(substring,"STOP") && keyword_accepted == true)
		{
			publish.data = "STOP";
			direction.publish(publish);
			cout<<str<<endl<<endl;
			str = "";
			speakerStack.push_back("Stopping");
			keyword_accepted = false;
		}
		else if(strstr(substring,"LEFT") && keyword_accepted == true)
		{
			publish.data = "LEFT";
			rotation.publish(publish);
			cout<<str<<endl<<endl;
			str = "";
			speakerStack.push_back("Turning Left");
			keyword_accepted = false;
		}
		else if(strstr(substring,"RIGHT") && keyword_accepted == true)
		{
			publish.data = "RIGHT";
			rotation.publish(publish);
			cout<<str<<endl<<endl;
			str = "";
			speakerStack.push_back("Turning Right");
			keyword_accepted = false;
		}
		else if(strstr(substring,"ONE") && keyword_accepted == true)
		{
			publish.data = ("A");
			destination.publish(publish);
			cout<<publish.data<<endl<<endl;
			str = "";
			speakerStack.push_back("Going to 1!");
			keyword_accepted = false;
		}
		else if(strstr(substring,"TWO") && keyword_accepted == true)
		{
			publish.data = ("B");
			destination.publish(publish);
			cout<<publish.data<<endl<<endl;
			str = "";
			speakerStack.push_back("Going to 2!");
			keyword_accepted = false;
		}
		else if(strstr(substring,"THREE") && keyword_accepted == true)
		{
			publish.data = ("C");
			destination.publish(publish);
			cout<<publish.data<<endl<<endl;
			str = "";
			keyword_accepted = false;
			speakerStack.push_back("Going to 3!");
		}
		else if(strstr(substring,"FOUR") && keyword_accepted == true)
		{
			publish.data = ("D");
			destination.publish(publish);
			cout<<publish.data<<endl<<endl;
			str = "";
			speakerStack.push_back("Going to 4!");
			keyword_accepted = false;
		}

		else if(strstr(substring,"TIME") && keyword_accepted == true)
		{
			//printf("Time\n");
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			int hour = tm.tm_hour;
			int minute = tm.tm_min;
			bool pm = false;
			string pmstr =" ";
			if(hour > 12)
			{
				hour -= 12;
				pm = true;
			}

			if(pm)
				pmstr = " PM";
			else
				pmstr = " AM";
			
			char timestr[100];
			sprintf(timestr, "The time is %d %d",hour,minute);

			speakerStack.push_back(((string)timestr + pmstr));
			str = "";
			keyword_accepted = false;
		}

		else if(strstr(str.c_str(),"EVA"))
		{
			speakerStack.push_back("Hello! How can I help you?");
			keyword_accepted = true;

			str = "";
		}

		else if(strstr(substring,"LOCATION") && keyword_accepted == true)
		{
			get_location = true;
			keyword_accepted = false;
			str = "";
		}

		else if(strstr(substring,"DOING") && keyword_accepted == true)
		{
			speakerStack.push_back("I am doing great!");
			keyword_accepted = false;
			str = "";
		}
	
		
		//speakerStack.push_back(str);	//this line will replay the audio command heard
		//str = "";

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
		
		if(speak == "0")
			key_press = "0";

		else if(speak == "1")
			key_press = "1";

		else if(speak != "")
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
