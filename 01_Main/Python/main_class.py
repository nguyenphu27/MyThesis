import os, os.path
import sys
import subprocess
# Define class
PROJECTPATH = "/home/pi/healthsystem/"

class MODULE:
    'Base class for all modules, used for inter-process communication'
	
    def __init__(self, name, path, existFlag=0):
        self.name = name
        self.path = path
        self.existFlag = existFlag
        self.fullpath = os.path.join(self.path, self.name)
        
    def isExist(self):
        self.existFlag = os.path.isfile(self.fullpath)
        return self.existFlag
        
    def create(self):
        if self.existFlag:
            print "%s already exist, Error: E101" %self.fullpath
            raise
        else:
            file_stream = open(self.fullpath,'w')
            file_stream.close()
            
    def delete(self):
        if self.existFlag:
            os.remove(self.fullpath)
        else:
            print "%s is not exist, Error: E102" %self.fullpath
            raise
            
    def call(self):
        #if self.existFlag:
        if 1:
            subprocess.Popen(self.fullpath)
            print "call %s" %self.fullpath
        else:
            print "%s is not exist, Error: E102" %self.fullpath
class File(MODULE):
    'Class for communication files'
    def call(self):
        return

class File_finish(File):
    'Class for all files named "_finish"'
    
    finish_count = 0
    
    def __init__(self, name, path, existFlag=0):
        self.name = name
        self.path = path
        self.existFlag = existFlag
        self.fullpath = os.path.join(self.path, self.name)
        File_finish.finish_count+=1
    
class File_start(File):
    'Class for all files named "_start"'
    
    start_count = 0
    
    def __init__(self, name, path, existFlag=0):
        self.name = name
        self.path = path
        self.existFlag = existFlag
        self.fullpath = os.path.join(self.path, self.name)
        File_start.start_count+=1

class File_result(File):
    'Class for all files named "_result"'
    
    result_count = 0
    
    def __init__(self, name, path, existFlag=0):
        self.name = name
        self.path = path
        self.existFlag = existFlag
        self.fullpath = os.path.join(self.path, self.name)
        File_result.result_count+=1

#audio_file = File("audio","/home/pi/")
sample_process_1 = MODULE("sample_process_1",PROJECTPATH)
sample_process_2 = MODULE("sample_process_2",PROJECTPATH)
#sample_process_1.__File__call()
#print sample_process_2.fullpath
sample_process_2.call()
#subprocess.Popen(r'/home/pi/healthsystem/sample_process_2')

