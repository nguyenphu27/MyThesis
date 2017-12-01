# pylint: disable=C0326, C1001
"""AUDIO
Author: Huy Vu
Date: Mar 20, 2017
Update: May 18, 2017"""

from time import sleep
from main_class import FLOW
from audio_class import MP3
import os, os.path

HEALTHSYSTEM = FLOW()
MP3LIST = {'welcome_voice':MP3('welcome_voice'),\
        'touch_voice':MP3('touch_voice', sublist=['touch_finish']),\
        'scale_voice':MP3('scale_voice', sublist=['scale_start']),\
        'scale_start_voice':MP3('scale_start_voice', sublist=['scale_start']),
        'end_voice_demo':MP3('end_voice',sublist=['result'])\
        #'end_voice':MP3('end_voice', sublist=['spo2_result', 'scale_result',\
        #                 'temp_result','height_result','bpressure_result'])\
            }
# Check audio modules existence

# START voice
MP3LIST['welcome_voice'].play()
# Sleep to free resource

sleep(2)

# wait for scale_start file
# while HEALTHSYSTEM.scale.start.is_exist() == 0:
#     sleep(1)
#     pass
# MP3LIST['scale_start_voice'].play()

while 1:
    if os.path.exists("result"):
        MP3LIST['end_voice_demo'].play()
        if MP3LIST['end_voice_demo'].was_played_flag==1:
            print 'Audio OFF'
            break
# Says sth when all result presented

