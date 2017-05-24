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

# setup audio
#if HEALTHSYSTEM.audio.exist_flag:
if 1:
    print "Setup audio system"
    HEALTHSYSTEM.audio.call()

# setup bluetooth
#if HEALTHSYSTEM.bluetooth.exist_flag:
    print "Setup bluetooth service"
    HEALTHSYSTEM.bluetooth.call()

# initialize remain modules
# initialize scale
#if HEALTHSYSTEM.scale.exist_flag==1 or 1:
    print "Initializing Scale module"
    HEALTHSYSTEM.scale.call()
#    os.system("python3 scale.py")

# Note: at this point, spo2, scale, temperature modules will wait for signal-file
# e.g. spo2 wait for file scale_finish and temp_finish

# Measuring....
# Still measuring....
# Finish
# Status: pi has spo2_result, spo2_finish
# with spo2_finish, bpressure machine will start to measure

# Measuring BPressure...
# ......................
# Finish
# Now pi has bpressure_result
# And from other modules, pi also has: spo2_result, scale_result,
# temp_result, height_result(from uno)

# when exist 5 files above, call wifi module to send data to server
while 1:
    sleep(1)
    RESULT =  HEALTHSYSTEM.scale.result.is_exist()

# send result to server by calling wifi module
    if RESULT:
        print "Preparing result"
        prepare_result()
        print "Call WIFI module for sending data to server"
        #HEALTHSYSTEM.wifi.call()
        break

# NOTE: audio will automatically call when exist appropriate file
# NOTE: this flow only run once, will upgrade to run repeatedly when once-running correctly
# END
