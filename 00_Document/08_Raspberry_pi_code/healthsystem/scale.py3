####################################################################
### This is code for plug-and-play scale module
### using python 3.5 with auto-recognition port and
### timeout for crash or errors safety reason features
####################################################################

import serial
import os, os.path
from time import sleep

serial_port = None
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "scale_result";
make_this_file = "scale_start"
result=list()

if os.path.exists("scale_port"):
	with open("scale_port","r") as f:
		serial_port=f.read()
		f.close()

checking_timeout = 0

if os.path.exists("scale_stop"):
	os.remove("scale_stop")

if os.path.exists("scale_result"):
	os.remove("scale_result")

try:
    ser = serial.Serial(serial_port, baud_rate, timeout=0.5)
    count = 0
    delay_sample = 0
    if ser.isOpen():
        while(1):
            ser.write(bytes("start", 'UTF-8'))
            a=ser.readline().rstrip().decode()
            sleep(1)
            if a=='start':
                print("start module scale")
                checking_timeout=0
                break
            checking_timeout+=1
            if checking_timeout == 40:
                break
        i=0
        sleep(1)
        if checking_timeout==0: #timeout safe switch for module
            while True:
                ser.write(bytes("  ", 'UTF-8'))
                line = ser.readline().rstrip().decode()
                sleep(1)
                try:
                    if line != '' and 'start' not in line and float(line) > 0:
                        print(line+'kgs')
                        result.append(float(line))
                        if i>=2:
                            diff_0 = (result[i-1] - result[i]) / (result[i])
                            diff_1 = (result[i-2] - result[i]) / (result[i])
                            if abs(diff_0)<0.02 and abs(diff_1)<0.02:
                                output_file = open(write_to_file_path, "w+")
                                output_file.write(str(result[i]))
                                output_file.close()
                                break
                        i+=1
                except:
                    pass
                checking_timeout+=1
                if checking_timeout == 20:
                    break

            while(1):
                ser.write(bytes("stop", 'UTF-8'))
                a=ser.readline().rstrip().decode()
                sleep(2)
                if a=='stop':
                    print("stop module scale")
                    break
        
        with open("scale_stop","w") as f:
            f.write("stop")
            f.close
    ser.close() #close serial
except serial.serialutil.SerialException as er:
    with open("scale_stop","w") as f:
        f.write("stop")
        f.close
    print(er)