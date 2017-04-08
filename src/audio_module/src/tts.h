#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_OF_HEADER 44
#define APP_DIR "/home/pi/catkin_ws/src/audio_module/src/"

void make_audio(char* text);
void single_channel_wav_to_dual_channel_raw();
void play_audio();
