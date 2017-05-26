# pylint: disable=C0326, C1001
"""AUDIO CLASS
Author: Huy Vu
Date: Mar 20, 2017
Update: May 18, 2017"""

import os
import os.path
import subprocess
from platform import platform
from main_class import File
# Detect Project's path (windows version is here only for testing, will be remove when stable
if 'Linux' in platform():
    PROJECTPATH = r"/home/pi/healthsystem/"
    MP3_SUFFIX = r"mp3/"
else:
    PROJECTPATH = r"D:\00_work\local\01_healthsystem"
    MP3_SUFFIX = r"mp3\\"
MP3PATH = os.path.join(PROJECTPATH, MP3_SUFFIX)

# Define class
class MP3:
    'Class for an MP3 File'
    mp3_count = 0

    def __init__(self, name, path=MP3PATH, filetype=".mp3", sublist=[]):
        self.name = name + filetype
        self.filetype=filetype
        self.path = path
        self.fullpath = os.path.join(self.path, self.name)
        print self.fullpath
        self.exist_flag = self.is_exist()
        self.was_played_flag = 0
        if self.exist_flag==0:
            self.missing()
        MP3.mp3_count+=1
        self.sub = []
        self.subcount = 0
        for item in sublist:
            self.sub.append(File(item))
            self.subcount+=1
        return

    def is_exist(self):
        "Check module's existence"
        self.exist_flag = os.path.isfile(self.fullpath)
        return self.exist_flag

    def missing(self):
        "Print the missing message"
        MP3.mp3_count-=1
        print "Missing MP3 File %s, E105" %self.name

    def play(self):
        "Play this mp3 file"
        # argument for playing mp3
        args=['omxplayer', self.fullpath]
        # if there is condition, check
        if self.sub != []:
            for item in self.sub:
                if item.is_exist()==0:
                    return
        if self.was_played_flag==1:
            return
        if 'Linux' in platform():
           # while 1:
           #     try:
           #         exit_code = subprocess.check_output(['omxplayer_wrapper.sh'])
           #         if exit_code == 0:
           #            # no omxplayer is running
           #             subprocess.Popen(args)
           #             print self.name, 'is playing'
           #             break
           #     except Exception as err:
           #         print err

            subprocess.Popen(args)
            #print self.name, 'is playing'
        else:
            pass
        self.was_played_flag=1
