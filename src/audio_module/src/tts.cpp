#include "tts.h"

void single_channel_wav_to_dual_channel_raw()
{
 char* wav_filename;// = (char*)"temp/test.wav";
 char* raw_filename;// = (char*)"temp/test.raw"; 

 sprintf(wav_filename,"%stemp/test.wav",APP_DIR);
 sprintf(raw_filename,"%stemp/test.raw",APP_DIR);

 char* header;
 char* data;

 header = (char*) malloc(SIZE_OF_HEADER); 
 data = (char*) malloc(sizeof(char)*2); 

 FILE *ptr = fopen(wav_filename,"rb");
 if (ptr == NULL) 
 {
	printf("Error opening wav file %s\n",wav_filename);
	exit(1);
 }

  
 int read = 0;
 
 if(!(read = fread(header, SIZE_OF_HEADER, 1, ptr)))
  printf("File is empty..\n");

 FILE *ptr_raw = fopen(raw_filename,"w");

 if (ptr_raw == NULL) 
 {
   printf("Error opening raw file\n");
	exit(1);
 }

 //int counts = 0;
 while(read = fread(data, 2, 1, ptr))
 {
   fwrite(data,2,1,ptr_raw);
   fwrite(data,2,1,ptr_raw);
 }

 fclose(ptr_raw);
 fclose(ptr);

 free(header);	

}

void make_audio(char* text)
{
	char command[1000];
	sprintf(command,"pico2wave -w %stemp/test.wav \"%s\"",APP_DIR,text);
	printf("creating wav file: %s\n",command);
	system(command);
	single_channel_wav_to_dual_channel_raw();
}

void play_audio()
{
	char command[1000];
	sprintf(command,"aplay -Dhw:sndrpiwsp -r 16000 -c 2 -f S16_LE %stemp/test.raw",APP_DIR);	
	system(command);
}