import os, os.path
import sys
import subprocess
# Define class
#PROJECTPATH = "/home/pi/healthsystem/"
PROJECTPATH = "D:\99_repo\99_local"

class MODULE:
    'Base class for all modules, used for inter-process communication'
	
    def __init__(self, name, path=PROJECTPATH, existFlag=0, start=0, finish=0, result=0):
        self.name = name
        self.path = path
        self.fullpath = os.path.join(self.path, self.name)
        self.existFlag = self.isExist()
        if existFlag==0:
            print "Missing Module %s, E104" %self.name

        self.finish=File_finish(self.name)
        self.start=File_start(self.name)
        self.result=File_result(self.name)
        
    def isExist(self):
        self.existFlag = os.path.isfile(self.fullpath)
        return self.existFlag
        
    def call(self):
        #if self.existFlag:
        if 1:
            subprocess.Popen(self.fullpath)
            print "call %s" %self.fullpath
        else:
            print "%s is not exist, Error: E102" %self.fullpath

class File():
    'Class for communication files'
    def __init__(self, name, path=PROJECTPATH, existFlag=0):
        self.name = name
        self.path = path
        self.existFlag = existFlag
        self.fullpath = os.path.join(self.path, self.name)

    def create(self):
        if self.existFlag:
            print "%s already exist, Error: E101" %self.fullpath
        else:
            file_stream = open(self.fullpath,'w')
            file_stream.close()

    def delete(self):
        if self.existFlag:
            os.remove(self.fullpath)
        else:
            print "%s is not exist, Error: E102" %self.fullpath

    def isExist(self):
        self.existFlag = os.path.isfile(self.fullpath)
        return self.existFlag

class File_finish(File):
    'Class for all files named "_finish"'
    
    finish_count = 0
    
    def __init__(self, name):
        File.__init__(self,name+'_finish')
        File_finish.finish_count+=1

    
class File_start(File):
    'Class for all files named "_start"'
    
    start_count = 0
    
    def __init__(self, name):
        File.__init__(self,name+'_start')
        File_start.start_count+=1

class File_result(File):
    'Class for all files named "_result"'
    
    result_count = 0
    
    def __init__(self, name):
        File.__init__(self,name+'_result')
        File_result.result_count+=1

class FLOW:
    'Base class handles flow information'
    
    def __init__(self):
        

