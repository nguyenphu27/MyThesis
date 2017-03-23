# pylint: disable=C0326, C1001
"""AUDIO CLASS
Author: Huy Vu
Date: Mar 20, 2017"""

import os
import os.path
import subprocess
from platform import platform
from main_class import File
# Detect Project's path (windows version is here only for testing, will be remove when stable
if 'Linux' in platform():
    PROJECTPATH = r"/home/pi/healthsystem/"
else:
    PROJECTPATH = r"D:\99_repo\99_local"
MP3_SUFFIX = r"mp3"
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
            try:
                subprocess.Popen(args)
                print self.name, 'is playing'
            except Exception as err:
                print err
        else:
            print self.name, 'is playing'
        self.was_played_flag=1
