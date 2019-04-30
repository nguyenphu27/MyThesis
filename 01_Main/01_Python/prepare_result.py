# pylint: disable=C0326, C1001
"""PREPARE result
Author: Huy Vu
Date: Mar 27, 2017
Update: Mar 27, 2017"""
import json
from time import ctime
from time import time
def prepare_result():
    "Collecting data from result files and format the data"
    print "Can do chieu cao, can nang, do huyet ap, thu suc keo, ..."
    filestream = list()
    # Can do chieu cao, can nang, do huyet ap, ...
    try:
        filestream.append(open('bluetooth_result','r'))
    except:
        filestream.append('30:75:12:A0:74:5A')

    try:
        filestream.append(open('height_result','r'))
    except:
        filestream.append('0')

    try:
        filestream.append(open('scale_result','r'))
    except:
        filestream.append('0')

    try:
        filestream.append(open('bpressure_result','r'))
    except:
        filestream.append('0')

    try:
        filestream.append(open('spo2_result','r'))
    except:
        filestream.append('0')

    try:
        filestream.append(open('temp_result','r'))
    except:
        filestream.append('0')

    collected=list()
    mID = int(time())
    mtime = ctime()
    for item in filestream:
        try:
            collected.append(item.read())
        except AttributeError:
            collected.append(item)
        try:
            item.close()
        except AttributeError:
            pass

    with open("result","a") as result:
        json.dump({'id':mID, 'time':mtime, \
                    'macid':collected[0],\
                    'height':collected[1],\
                    'weight':collected[2],\
                    'spo2':collected[3],\
                    'bloodpressure':collected[4],\
                    'temp':collected[5]\
                    },result,sort_keys=True)
        result.write('\n')
        result.close()

prepare_result()