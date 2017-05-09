This is a threading based application to avoid delays in the functionality. It have 4 threads in the main application which are as follows:-

Thread 1: (Hearing Thread)
Continuously listens to the audio input.
Converts Audio to the text.
Parse the required text commands (fetch the important keywords).
Publish the command to ROS topic.

Thread 2: (Speaking Thread)
Fetches the text commands from ROS topics published by the Navigation and Video Module.
Decode the commands and create the meaningful sentences to speak.
Convert the sentences to the audio file and play the output.

Thread 3: (Data Analysis thread)
Generates the audio wav file from the data received in the continuously listening mode.
Audio file is updated at every interval of 10 seconds.
Converts the WAV file to RAW file.
Fetches the PCM data from the RAW file and creates a data file.
Plots the spectrum at runtime.(Currently we can get the sound spectrum by manually by generating the graph. We are figuring out a way to generate it on runtime.)

Thread 4: (ROS Publisher Subscriber)
Publish the data received from audio device.
Listen to the data received from the master module (Video module in this case).

The voice training, language model, acoustic model and evaluation data created for this project can be found in the following link:
https://drive.google.com/drive/folders/0B5OW6nWK7t5udVFBQ2dvR3lwSWc?usp=sharing

Demo videos can be found in the following link:
https://drive.google.com/drive/folders/0B5OW6nWK7t5uT0dsMElhNHVRaDQ?usp=sharing

Steps to run the project are:
(Make sure all the dependencies are already installed.)
1. Open terminal (ctrl + shift + t)
2. cd catkin_ws
3. source devel/setup.bash
4. rosrun audio_module audio

To start speech recognition press 1 and enter. and say "Hey Eva".
Commands that the robot can understand are:
1. Hey Eva
2. How are you doing?
3. What is the time?
4. What is my location?
5. Take me to 1/2/3/4
6. Go to 1/2/3/4
7. Move forward
8. Move backward
9. Turn right
10. Turn Left
11. Stop

Apart from responding to these querries, the robot also speaks the following information:
1. Number of faces detected.
2. Number of objects detected.
3. Number of objects within 2 meters.
4. System Stated greeting.


