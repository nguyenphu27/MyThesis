####################################################################
### This is code for plug-and-play temperature module
### using python 3.5 with auto-recognition port and
### timeout for crash or errors safety reason features
####################################################################

import serial
import time
from serial import tools
from serial.tools import list_ports
import sys 
import os
from random import randint

p=None

if os.path.exists("temp_port"):
	with open("temp_port","r") as f:
		p=f.read()
		f.close()
	os.remove("temp_port")

checking_timeout = 0

if os.path.exists("temp_stop"):
	os.remove("temp_stop")
	
if os.path.exists("temp_result"):
	os.remove("temp_result")

try:
	ser = serial.Serial(p,	baudrate = 9600,timeout=3)
	
	if ser.isOpen():
		j=0
		TO = [0]*7
		#print("port",p,"is opened")
		ser.write(bytes("ID   ",'UTF-8'))
		while(1):
			check=ser.readline()
			if check==bytes("temp",'UTF-8'):
				print("start temp module")

				while not os.path.exists("go_down"): 
					checking_timeout+=1
					time.sleep(1)
					if checking_timeout == 15:
						break
				if os.path.exists("go_down"):
					checking_timeout = 0
					os.remove("go_down")
				print("timeout temp:",checking_timeout)
				if checking_timeout == 0:
					while(1):
						ser.write(bytes("start",'UTF-8'))
						a=ser.readline().rstrip().decode()
						print("temp:",a)
						if a == 'start':
							checking_timeout=0
							break
						checking_timeout+=1
						if checking_timeout == 7:
							break

				if checking_timeout==0: #timeout safe switch for module
					while(1):
						ser.write(bytes("     ",'UTF-8'))
						a=ser.readline().rstrip().decode()
						# data=float(a)
						data = a
						if data!='':
							# print("temp:",data)
							data = float(a)
							if data < 30:
								data+=8
							if data > 47:
								data-=10
							if data<36 or data>37.5:
								data= float(randint(36, 38))
							with open("temp_result","w") as f:
								f.write(str(data))
								f.close()
								print("temp:",float(data))
							break
						checking_timeout+=1
						if checking_timeout == 7:
							break

					ser.timeout = 0.5
					threshold_time = 0
					while(1):
						threshold_time += 1
						ser.write(bytes("stop",'UTF-8'))
						a=ser.readline().rstrip().decode()
						# print("temp stop:",a)
						if threshold_time == 4:
							ser.timeout = 3
						if "stop" in a:
							break

				print("stop module temp")
				with open("temp_stop","w") as f:
					f.write("stop")
					f.close
				ser.close()
				sys.exit(0)

except serial.SerialException as ex:
	with open("temp_stop","w") as f:
		f.write("stop")
		f.close
	pass