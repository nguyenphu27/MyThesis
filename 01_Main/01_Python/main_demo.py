#  pylint: disable=C0326, C1001
"""MAIN PROGRAM OF THE SYSTEM
Author: Huy Vu
Date: Mar 18, 2017
Update: May 24, 2017"""
# START
from time import sleep
from main_class import FLOW
import os
import subprocess
from prepare_result import prepare_result
HEALTHSYSTEM = FLOW()

# wait for touch module send start signal
# ... 


# setup audio
if HEALTHSYSTEM.audio.exist_flag:
    print "Setup audio system"
    HEALTHSYSTEM.audio.call()

# setup bluetooth
if HEALTHSYSTEM.bluetooth.exist_flag:
    print "Setup bluetooth service"
    HEALTHSYSTEM.bluetooth.call()

# initialize scale
if HEALTHSYSTEM.scale.exist_flag:
    print "Initializing Scale module"
    HEALTHSYSTEM.scale.call()

while 1:
    try:
        sleep(1)
        RESULT =  HEALTHSYSTEM.scale.result.is_exist()

    # send result to server by calling wifi module
        if RESULT:
            print "Preparing result"
            prepare_result()
            print "Call WIFI module for sending data to server"
            #HEALTHSYSTEM.wifi.call()
            break
    except KeyboardInterrupt:
        print '\nCatched Ctrl C'
        break
# END
