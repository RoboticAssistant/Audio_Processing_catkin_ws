#!/bin/bash

pocketsphinx_batch \
        -adcin yes \
 	-cepdir /home/pi/catkin_ws/src/audio_module/src/Eva_Model/test/ \
	-cepext .wav \
	-ctl test.fileids \
	-lm 8672.lm.bin \
	-dict 8672.dic \
	-hmm en-us-adapt \
	-hyp test.hyp	

./word_align.pl test.transcription test.hyp
