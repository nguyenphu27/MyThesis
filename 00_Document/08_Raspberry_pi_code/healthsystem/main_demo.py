#  pylint: disable=C0326, C1001
"""MAIN PROGRAM OF THE SYSTEM
Author: Huy Vu
Date: Mar 18, 2017
Update: Oct 27, 2017"""
# START
from time import sleep
from main_class import FLOW
import os, os.path
import subprocess
import usb.core, usb.util
from check_port import check_port
from prepare_result import prepare_result
import RPi.GPIO as g
from time import sleep
HEALTHSYSTEM = FLOW()

# gpio setup for touch
g.setmode(g.BCM)
g.setup(4,g.IN,pull_up_down=g.PUD_DOWN)

state1=0
state2=0

# initialize touch panel for moving table up and down
HEALTHSYSTEM.touch.call()

while(1):

    if os.path.exists("/home/pi/healthsystem/result"):
        os.remove("/home/pi/healthsystem/result")

    sleep(3)
            
    # check each port looking for modules
    check_port()

    # setup audio
    if HEALTHSYSTEM.audio.exist_flag:
        print "Setup audio system"
        HEALTHSYSTEM.audio.call()

    sleep(1)
    print("please press start to run")
    print("state before run: state2:",state2,"state1:",state1)
    # check start signal from touch
    state1= not state2
    while(1):
        # state1 = g.input(4)
        if state1 != state2:
            print("start running")
            break

    c_spo2=0
    c_temp=0
    c_scale=0
    c_height=0

    # setup bluetooth
    if os.path.exists("/home/pi/healthsystem/bluetooth_result"): 
            os.remove("/home/pi/healthsystem/bluetooth_result")
    if HEALTHSYSTEM.bluetooth.exist_flag:
        
        print "Setup bluetooth service"
        HEALTHSYSTEM.bluetooth.call()
        while HEALTHSYSTEM.bluetooth.result.is_exist()==0:
            sleep(0.5)
            pass

    sleep(3)

    # initialize height
    if HEALTHSYSTEM.height.exist_flag:
        if os.path.exists("/home/pi/healthsystem/height_port"):
            c_height=1
            print "Initializing Height module"
            HEALTHSYSTEM.height.call()

    # initialize spo2
    if HEALTHSYSTEM.spo2.exist_flag:
        if os.path.exists("/home/pi/healthsystem/spo2_port"):
            c_spo2=1
            print "Initializing Spo2 module"
            HEALTHSYSTEM.spo2.call()

    # initialize temp
    if HEALTHSYSTEM.temp.exist_flag:
        if os.path.exists("/home/pi/healthsystem/temp_port"):
            c_temp=1
            print "Initializing Temp module"
            HEALTHSYSTEM.temp.call()

    # initialize scale
    if HEALTHSYSTEM.scale.exist_flag:
        if os.path.exists("/home/pi/healthsystem/scale_port"):
            c_scale=1
            print "Initializing Scale module"
            HEALTHSYSTEM.scale.call()

    r_temp = r_spo2 = r_scale = r_height = r_hr = 0
    while 1:
        try:
            
            if c_temp:
                while not r_temp:
                    r_temp = HEALTHSYSTEM.temp.stop.is_exist()
            if c_spo2:
                while not r_spo2:
                    r_spo2 =  HEALTHSYSTEM.spo2.stop.is_exist()
            if c_scale:
                while not r_scale:
                    r_scale = HEALTHSYSTEM.scale.stop.is_exist()
            if c_height:
                while not r_height:
                    r_height = HEALTHSYSTEM.height.stop.is_exist()

            if (r_temp and c_temp) + (r_spo2 and c_spo2) + (r_scale and c_scale) + (r_height and c_height):
                print "Preparing result"
                prepare_result()
                print "Call WIFI module for sending data to server"

                
                if r_temp and os.path.exists("/home/pi/healthsystem/temp_result"): 
                    os.remove("/home/pi/healthsystem/temp_result")

                if r_spo2 and os.path.exists("/home/pi/healthsystem/spo2_result"): 
                    os.remove("/home/pi/healthsystem/spo2_result")

                if r_scale and os.path.exists("/home/pi/healthsystem/scale_result"):
                    os.remove("/home/pi/healthsystem/scale_result")
                    os.remove("/home/pi/healthsystem/scale_port")

                if r_hr and os.path.exists("/home/pi/healthsystem/hr_result"): 
                    os.remove("/home/pi/healthsystem/hr_result")

                if r_height and os.path.exists("/home/pi/healthsystem/height_result"): 
                    os.remove("/home/pi/healthsystem/height_result")

                if r_temp:
                    while not os.path.exists("/home/pi/healthsystem/temp_stop"): continue
                    os.remove('/home/pi/healthsystem/temp_stop')
                if r_spo2:
                    while not os.path.exists("/home/pi/healthsystem/spo2_stop"): continue
                    os.remove('/home/pi/healthsystem/spo2_stop')
                if r_scale:
                    while not os.path.exists("/home/pi/healthsystem/scale_stop"): continue
                    os.remove('/home/pi/healthsystem/scale_stop')
                if r_hr:
                    while not os.path.exists("/home/pi/healthsystem/height_stop"): continue
                    os.remove('/home/pi/healthsystem/height_stop')
                break
            if c_height==0 and c_scale==0 and c_spo2==0 and c_temp==0:
                prepare_result()
                break

        except KeyboardInterrupt:
            print '\nCatched Ctrl C'
            break

            os.system('pgrep python > pidlist')
            f = open('./pidlist','r')
            for i in f:
                print 'kill',
                print i
                try:
                    os.system('sudo kill ' + i +'>./log/kill_log')
                except:
                    pass
    # END
    state2=g.input(4)
    print("finished")
    print("state after finished: state2:",state2,"state1:",state1)
