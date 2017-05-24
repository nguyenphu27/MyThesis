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
        'scale_voice':MP3('scale_voice'),\
        'end_voice':MP3('end_voice', sublist=['spo2_result', 'scale_result',\
                         'temp_result','height_result','bpressure_result'])}
# Check audio modules existence
if HEALTHSYSTEM.audio.is_exist() == 0:
    print "Missing Module Audio, E104"

else:
    # START voice
    MP3LIST['main_voice'].play()
    # Sleep to free resource
    # Poll every 2secs
    sleep(10)
    print 'Measuring bodyweight'
    MP3LIST['scale_start_voice'].play()
    while 1:
        sleep(3)
        RESULT =  HEALTHSYSTEM.scale.result.is_exist()
        if RESULT:
            MP3LIST['scale_finish_voice'].play()
            break
    # Says sth when all result presented
    MP3LIST['end_voice'].play()

