#  pylint: disable=C0326, C1001
"""MAIN PROGRAM OF THE SYSTEM"""
# START
from time import sleep
from main_class import FLOW

HEALTHSYSTEM = FLOW()

# setup audio
if HEALTHSYSTEM.audio.exist_flag:
    print "Setup audio system"
    HEALTHSYSTEM.audio.call()

# setup bluetooth
if HEALTHSYSTEM.bluetooth.exist_flag:
    print "Setup bluetooth service"
    HEALTHSYSTEM.bluetooth.call()

# power bpressure machine on
if HEALTHSYSTEM.bpressure.sub[0].exist_flag==1:
    print "Power Blood pressure machine ON"
    HEALTHSYSTEM.bpressure.sub[0].call()

# initialize remain modules
# initialize spo2
if HEALTHSYSTEM.spo2.exist_flag==1:
    print "Initializing SpO2 module"
    HEALTHSYSTEM.spo2.call()

# initialize scale
if HEALTHSYSTEM.scale.exist_flag==1:
    print "Initializing Scale module"
    HEALTHSYSTEM.scale.call()

# initialize temperature
if HEALTHSYSTEM.temp.exist_flag==1:
    print "Initializing Temperature module"
    HEALTHSYSTEM.temp.call()

# Note: at this point, spo2, scale, temperature modules will wait for signal-file
# e.g. spo2 wait for file scale_finish and temp_finish

# setup touch
if HEALTHSYSTEM.touch.exist_flag==1:
    print "Enabling Touch system"
    HEALTHSYSTEM.touch.call()

# now touch module will create a file name touch_finish,
# and with that file, uno module will be called and send START signal to the uno
# then, the uno measures height
if HEALTHSYSTEM.uno.exist_flag==1:
    print "Sending START to Arduino Uno"
    HEALTHSYSTEM.uno.call()
# Pi also measures scale & temperature concurrently at this point
# When scale generates scale_finish and temperature generates temp_finish
# SpO2 will start to measure

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
    RESULT = HEALTHSYSTEM.uno.sub[0].result.is_exist() and\
            HEALTHSYSTEM.spo2.result.is_exist() and\
            HEALTHSYSTEM.scale.result.is_exist() and\
            HEALTHSYSTEM.temp.result.is_exist() and\
            HEALTHSYSTEM.bpressure.result.is_exist()

# send result to server by calling wifi module
    if RESULT:
        print "Call WIFI module for sending data to server"
        HEALTHSYSTEM.wifi.call()
        break

# NOTE: audio will automatically call when exist appropriate file
# NOTE: this flow only run once, will upgrade to run repeatedly when once-running correctly
# END
