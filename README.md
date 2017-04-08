We are implementing threading based application to avoid delays in the functionality. We’ll have 3 threads in our main application which are as follows:-


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
