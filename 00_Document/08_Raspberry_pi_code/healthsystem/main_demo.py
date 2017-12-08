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

    if os.path.exists("result"):
        os.remove("result")
        
    # check each port looking for modules
    check_port()

    # setup audio
    if HEALTHSYSTEM.audio.exist_flag:
        print "Setup audio system"
        HEALTHSYSTEM.audio.call()

    sleep(1)
    print("please press start to run")
    # check start signal from touch
    if state1==state2:
        while(1):
            state1=g.input(4)
            if state1!= state2:
                print("start running")
                break
    else:
        while(1):
            state1=g.input(4)
            if state1!= state2:
                print("start running")
                break

    c_spo2=0
    c_temp=0
    c_scale=0
    c_height=0

    # setup bluetooth
    #if HEALTHSYSTEM.bluetooth.exist_flag:
    #    print "Setup bluetooth service"
    #    HEALTHSYSTEM.bluetooth.call()
    #    while HEALTHSYSTEM.bluetooth.result.is_exist()==0:
    #        sleep(0.5)
    #        pass

    # initialize height
    if HEALTHSYSTEM.height.exist_flag:
        if os.path.exists("height_port"):
            c_height=1
            print "Initializing Height module"
            HEALTHSYSTEM.height.call()

    # initialize spo2
    if HEALTHSYSTEM.spo2.exist_flag:
        if os.path.exists("spo2_port"):
            c_spo2=1
            print "Initializing Spo2 module"
            HEALTHSYSTEM.spo2.call()

    # initialize temp
    if HEALTHSYSTEM.temp.exist_flag:
        if os.path.exists("temp_port"):
            c_temp=1
            print "Initializing Temp module"
            HEALTHSYSTEM.temp.call()

    # initialize scale
    if HEALTHSYSTEM.scale.exist_flag:
        if os.path.exists("scale_port"):
            c_scale=1
            print "Initializing Scale module"
            HEALTHSYSTEM.scale.call()

    r_temp = r_spo2 = r_scale = r_height = r_hr = 0
    while 1:
        try:
            if c_temp:
                while not r_temp:
                    r_temp = HEALTHSYSTEM.temp.result.is_exist()
            if c_spo2:
                while not r_spo2 or not r_hr:
                    r_spo2 =  HEALTHSYSTEM.spo2.result.is_exist()
                    r_hr = os.path.exists("hr_result")
            if c_scale:
                while not r_scale:
                    r_scale = HEALTHSYSTEM.scale.result.is_exist()
            if c_height:
                while not r_height:
                    r_height = HEALTHSYSTEM.height.result.is_exist()

            if (r_temp and c_temp) + (r_spo2 and c_spo2) + (r_scale and c_scale) + (r_height and c_height):
                print "Preparing result"
                prepare_result()
                print "Call WIFI module for sending data to server"

                if r_temp: os.remove("temp_result")
                if r_spo2: os.remove("spo2_result")
                if r_scale:
                    os.remove("scale_result")
                    os.remove("scale_port")
                if r_hr: os.remove("hr_result")

                if r_temp:
                    while not os.path.exists("temp_stop"): continue
                    os.remove('temp_stop')
                if r_spo2:
                    while not os.path.exists("spo2_stop"): continue
                    os.remove('spo2_stop')
                if r_scale:
                    while not os.path.exists("scale_stop"): continue
                    os.remove('scale_stop')
                if r_hr:
                    while not os.path.exists("height_stop"): continue
                    os.remove('height_stop')
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
