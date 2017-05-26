# pylint: disable=C0326, C1001
"""AUDIO
Author: Huy Vu
Date: Mar 20, 2017
Update: May 18, 2017"""


import os
import os.path
import subprocess
from subprocess import Popen, PIPE

p = subprocess.Popen(["ls", "non existent"], stdout=subprocess.PIPE)
print 'hello'
while 1:
    exit_code = p.communicate()[0]
    exit_code = p.returncode
    print exit_code
    if exit_code == 0:
        # no omxplayer is running
        subprocess.Popen('omxplayer /home/pi/healthsystem/mp3/end_voice.mp3')
        print 'end_voice is playing'
