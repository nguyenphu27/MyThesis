# pylint: disable=C0326, C1001
"""AUDIO
Author: Huy Vu
Date: Mar 20, 2017"""

from main_class import FLOW
from time import sleep
from audio_class import MP3

HEALTHSYSTEM = FLOW()
print "PRINT FROM AUDIO MODULE"
MP3LIST = {'main_voice':MP3('main_voice'),\
        'touch_voice':MP3('touch_voice')}
# Check audio modules existence
if HEALTHSYSTEM.audio.exist_flag == 0:
    print "Missing Module Audio, E104"

else:
    # START voice
    if MP3LIST['main_voice'].exist_flag:
        MP3LIST['main_voice'].was_played_flag=1
        MP3LIST['main_voice'].play()
    while 1:
        # Sleep to free resource
        # Poll every 2secs
        sleep(2)
        if HEALTHSYSTEM.touch.finish.exist_flag==1 and MP3LIST['touch_voice'].was_played_flag==0:
            MP3LIST['touch_voice'].was_played_flag=1
            MP3LIST['touch_voice'].play()
            print "touch_voice is playing"
