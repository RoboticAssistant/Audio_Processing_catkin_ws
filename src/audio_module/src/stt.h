#include <pocketsphinx.h>
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include "std_msgs/String.h"
#include <sstream>

using namespace std;

#define APP_DIR "/home/pi/catkin_ws/src/audio_module/src/"
#define MODELDIR "/home/pi/catkin_ws/src/audio_module/pocketsphinx/model"

int stt_init();
int stt_continuous(ad_rec_t *ad,int16 *adbuf,uint8 *utt_started,uint8 *in_speech,int32 *k);
char const *recognize(ad_rec_t *ad,int16 *adbuf,uint8 *utt_started,uint8 *in_speech,int32 *k);
