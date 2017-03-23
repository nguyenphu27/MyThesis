# pylint: disable=C0326, C1001
"""HEADER FOR MAIN PROGRAM"""

import os
import os.path
import subprocess
from platform import platform

# Detect Project's path (windows version is here only for testing, will be remove when stable
if 'Linux' in platform():
    PROJECTPATH = r"/home/pi/healthsystem/"
else:
    PROJECTPATH = r"D:\99_repo\99_local"

# Define class
class MODULE:
    'Base class for all modules, used for inter-process communication'
    module_count = 0

    def __init__(self, name, path=PROJECTPATH, filetype=".c", sublist=[]):
        if filetype==".c":
            self.name = name
        else:
            self.name = name + filetype
        self.filetype=filetype
        self.path = path
        self.fullpath = os.path.join(self.path, self.name)
        self.exist_flag = self.is_exist()
        if self.exist_flag==0:
            self.missing()
        self.finish=FileFinish(self.name)
        self.start=FileStart(self.name)
        self.result=FileResult(self.name)
        MODULE.module_count += 1
        self.sub = []
        self.subcount = 0
        for item in sublist:
            self.sub.append(SUBMODULE(item))
            self.subcount+=1

    def is_exist(self):
        "Check module's existence"
        self.exist_flag = os.path.isfile(self.fullpath)
        return self.exist_flag

    def call(self):
        "Call the module"
        if self.exist_flag:
            if self.filetype==".py":
                args=['python',self.fullpath]
                subprocess.Popen(args)
            else:
                subprocess.Popen(self.fullpath)
                print "call %s" %self.fullpath
        else:
            self.missing()

    def missing(self):
        "Print the missing message"
        print "Missing Module %s, E104" %self.name

class SUBMODULE(MODULE):
    'Class for submodule'
    def cons(self):
        'Test constructor of SUBMODULE'
        print "SUBMODULE: " + self.fullpath
    def missing(self):
        print "Missing Submodule %s, E105" %self.name

class File():
    'Class for communication files'
    def __init__(self, name, path=PROJECTPATH):
        self.name = name
        self.path = path
        self.fullpath = os.path.join(self.path, self.name)
        self.exist_flag = self.is_exist()

    def create(self):
        "Create the file for communication"
        if self.exist_flag:
            print "%s already exist, Error: E101" %self.fullpath
        else:
            file_stream = open(self.fullpath,'w')
            file_stream.close()

    def delete(self):
        "Delete the file"
        if self.exist_flag:
            os.remove(self.fullpath)
        else:
            print "%s is not exist, Error: E102" %self.fullpath

    def is_exist(self):
        "Check file's existence"
        self.exist_flag = os.path.isfile(self.fullpath)
        return self.exist_flag

class FileFinish(File):
    'Class for all files named "_finish"'

    finish_count = 0

    def __init__(self, name):
        File.__init__(self,name+'_finish')
        FileFinish.finish_count+=1

class FileStart(File):
    'Class for all files named "_start"'

    start_count = 0

    def __init__(self, name):
        File.__init__(self,name+'_start')
        FileStart.start_count+=1

class FileResult(File):
    'Class for all files named "_result"'

    result_count = 0

    def __init__(self, name):
        File.__init__(self,name+'_result')
        FileResult.result_count+=1

class FLOW:
    'Base class handles flow information'

    def __init__(self):
        self.audio = MODULE("audio", filetype=".py")
        self.bluetooth = MODULE("bluetooth")
        self.bpressure = MODULE("bpressure", sublist=["bpressure_poweron"])
        self.scale = MODULE("scale")
        self.spo2 = MODULE("spo2")
        self.temp = MODULE("temp")
        self.touch = MODULE("touch")
        self.uno = MODULE("uno", sublist=["height"])
        self.wifi = MODULE("wifi")

    def setup_bluetooth(self):
        'Call bluetooth module for preparing service to pair with user\'s phone'
        self.bluetooth.call()

    def setup_bpressure(self):
        'Call bpressure_poweron to power on the BP machine'
        return 1