# pylint: disable=C0326, C1001
"""AUDIO
Author: Huy Vu
Date: Mar 20, 2017
Update: Mar 23, 2017"""

from time import sleep
from main_class import FLOW
from audio_class import MP3

HEALTHSYSTEM = FLOW()
MP3LIST = {'main_voice':MP3('main_voice'),\
        'touch_voice':MP3('touch_voice', sublist=['touch_finish']),\
        'scale_voice':MP3('scale_voice'),\
        'end_voice':MP3('end_voice', sublist=['spo2_result', 'scale_result',\
                         'temp_result','height_result','bpressure_result'])}
# Check audio modules existence
if HEALTHSYSTEM.audio.exist_flag == 0:
    print "Missing Module Audio, E104"

else:
    # START voice
    MP3LIST['main_voice'].play()
    while 1:
        # Sleep to free resource
        # Poll every 2secs
        sleep(2)
        print 'sleeping...'
        print 'Measuring bodyweight'
        MP3LIST['scale_voice'].play()
        #MP3LIST['touch_voice'].play()
        # Says sth when all result presented
        MP3LIST['end_voice'].play()

