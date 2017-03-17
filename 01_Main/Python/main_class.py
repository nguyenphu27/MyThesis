import os, os.path
import sys
# Define class


class File:
    'Base class for all files used for inter-process communication'
	
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

audio_file = File("audio","/home/pi/")


