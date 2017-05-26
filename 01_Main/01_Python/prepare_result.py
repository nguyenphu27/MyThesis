# pylint: disable=C0326, C1001
"""PREPARE result
Author: Huy Vu
Date: Mar 27, 2017
Update: Mar 27, 2017"""

def prepare_result():
    "Collecting data from result files and format the data"
    print "Can do chieu cao, can nang, do huyet ap, thu suc keo, ..."
    filestream = list()
    # Can do chieu cao, can nang, do huyet ap, ...
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
    for item in filestream:
        try:
            collected.append(item.read())
        except AttributeError:
            collected.append('0')
        collected.append(',')
        try:
            item.close()
        except AttributeError:
            pass

    result= open('result','w')
    for each in collected[0:len(collected)-1]:
        result.write(each)
    result.close()

