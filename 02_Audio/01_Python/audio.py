# pylint: disable=C0326, C1001
"""AUDIO
Author: Huy Vu
Date: Mar 20, 2017
Update: May 18, 2017"""

from time import sleep
from main_class import FLOW
from audio_class import MP3

HEALTHSYSTEM = FLOW()
MP3LIST = {'welcome_voice':MP3('welcome_voice'),\
        'touch_voice':MP3('touch_voice', sublist=['touch_finish']),\
        'scale_voice':MP3('scale_voice', sublist=['scale_start']),\
        'end_voice':MP3('end_voice', sublist=['spo2_result', 'scale_result',\
                         'temp_result','height_result','bpressure_result'])}
# Check audio modules existence
if HEALTHSYSTEM.audio.is_exist() == 0:
    print "Missing Module Audio, E104"

else:
    # START voice
    MP3LIST['welcome_voice'].play()
    # Sleep to free resource
    # Poll every 2secs
    sleep(3)
    # wait for scale_start file
    while HEALTHSYSTEM.scale.start.is_exist() == 0:
        pass
    MP3LIST['scale_start_voice'].play()
    while 1:
        sleep(3)
        SCALE_RESULT =  HEALTHSYSTEM.scale.result.is_exist()
        if SCALE_RESULT and MP3LIST['scale_finish_voice'].was_played_flag==0:
            MP3LIST['scale_finish_voice'].play()
            break
    # Says sth when all result presented
    sleep(3)
    MP3LIST['end_voice'].play()

