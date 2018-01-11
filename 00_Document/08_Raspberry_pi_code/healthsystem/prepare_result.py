# pylint: disable=C0326, C1001
"""PREPARE result
Author: Huy Vu
Date: Mar 27, 2017
Update: Mar 27, 2017"""
import json
import requests

def prepare_result():
    "Collecting data from result files and format the data"
#    print "Can do chieu cao, can nang, do huyet ap, thu suc keo, ..."
    filestream = list()
    # Can do chieu cao, can nang, do huyet ap, ...

    try:
        filestream.append(open('bluetooth_result','r'))
    except:
        filestream.append('69:69:69:69:69:69')

    try:
        filestream.append(open('height_result','r'))
    except:
        filestream.append('0')

    try:
        filestream.append(open('scale_result','r'))
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

    try:
        filestream.append(open('bpressure_result','r'))
    except:
        filestream.append('0')

    try:
        filestream.append(open('hr_result','r'))
    except:
        filestream.append('0')

    collected=list()
    for item in filestream:
        try:
            collected.append(item.read())
        except AttributeError:
            collected.append('0')

        try:
            item.close()
        except AttributeError:
            pass

    print(collected)
    result={'macid':collected[0], 'height':collected[1],'weight':collected[2],
                    'spo2':collected[3],'temp':collected[4],'bloodpressure':collected[5], 'heartrate':collected[6]}
    
    with open("result","w") as f:
        f.write(str(result))
        f.close()

    # with open("result.log","a") as f:
    #     f.write("\n"+str(result))
    #     f.close()
        
    try:
        r = requests.post("http://18.218.77.52:3000/addinfor", data=result)
        # print(r.text)
    except requests.exceptions.RequestException as e:
        print(e)


    
