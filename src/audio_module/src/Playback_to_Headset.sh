
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1 Digital Volume' 116

amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1L Input 1' None
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1R Input 1' None
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1L Input 2' None
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1R Input 2' None
# Setup HPOUT1 input path and volume
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1L Input 1' AIF1RX1
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1L Input 1 Volume' 30#16
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1R Input 1' AIF1RX2
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1R Input 1 Volume' 30#16
# Unmute the HPOUT1 Outputs
amixer $1 -Dhw:sndrpiwsp cset name='HPOUT1 Digital Switch' on


# The following command can be used to test
# aplay -Dhw:sndrpiwsp -r 16000 -c 2 -f S16_LE test.raw



