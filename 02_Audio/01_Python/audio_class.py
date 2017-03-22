# pylint: disable=C0326, C1001
"""AUDIO CLASS
Author: Huy Vu
Date: Mar 20, 2017"""

import os
import os.path
import subprocess
# Define class
PROJECTPATH = r"/home/pi/healthsystem/"
#PROJECTPATH = r"D:\99_repo\99_local"
MP3_SUFFIX = r"mp3"
MP3PATH = os.path.join(PROJECTPATH, MP3_SUFFIX)
print MP3PATH

class MP3:
    'Class for an MP3 File'
    mp3_count = 0

    def __init__(self, name, path=MP3PATH, filetype=".mp3"):
        self.name = name + filetype
        self.filetype=filetype
        self.path = path
        self.fullpath = os.path.join(self.path, self.name)
        self.exist_flag = self.is_exist()
        self.was_played_flag = 0
        if self.exist_flag==0:
            self.missing()
        return

    def is_exist(self):
        "Check module's existence"
        self.exist_flag = os.path.isfile(self.fullpath)
        return self.exist_flag

    def missing(self):
        "Print the missing message"
        print "Missing MP3 File %s, E105" %self.name

    def play(self):
        "Play this mp3 file"
        args=['omxplayer', self.fullpath]
        subprocess.Popen(args)
