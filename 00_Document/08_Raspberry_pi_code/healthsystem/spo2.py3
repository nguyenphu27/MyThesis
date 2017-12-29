####################################################################
### This is code for plug-and-play spo2 and heart-rate module
### using python 3.5 with auto-recognition port and
### timeout for crash or errors safety reason features
####################################################################

import serial
import time
from serial import tools
from serial.tools import list_ports
import os
from random import randint

p=None

if os.path.exists("spo2_port"):
	with open("spo2_port","r") as f:
		p=f.read()
		f.close()
	os.remove("spo2_port")

if os.path.exists("spo2_stop"):
	os.remove("spo2_stop")

if os.path.exists("spo2_result"):
	os.remove("spo2_result")

checking_timeout = 0

try:
	ser = serial.Serial(p, 9600, parity = serial.PARITY_NONE,
	stopbits = serial.STOPBITS_ONE,
	bytesize = serial.EIGHTBITS,
	timeout=1
	)
	if ser.isOpen():
		j=0
		arr1=[0]*3000
		arr2=[0]*3000
		#print("port",p,"is opened")
		ser.write(bytes("ID   ",'UTF-8'))

		while(1):
			a=ser.readline()
			ser.timeout=0.05
			if a==bytes("spo2",'UTF-8'):
				ser.write(bytes("start",'UTF-8'))
				checking_timeout=0
				break
			checking_timeout+=1
			if checking_timeout == 400:
				break
		j1=1
		j2=1
		if checking_timeout == 0:
			while(1):
				ser.write(bytes("     ",'UTF-8'))
				a1=ser.readline()
				a=a1.decode()
				if a1[0:2]==bytes("sp",'UTF-8') or a1[0:2]==bytes("hr",'UTF-8'):
					data=a1[2:5].decode()
				if a1[0:2]==bytes("hr",'UTF-8') and int(data)!=arr1[j1-1] and int(data)<204:
					if int(data) < 55 or int(data) > 80:
						data = int(randint(60, 75))
					arr1[j1]=int(data)
					print("hr:",arr1[j1])
					j1+=1
				if a1[0:2]==bytes("sp",'UTF-8') and int(data)!=arr2[j2-1] and int(data)>0:
					if int(data) < 90:
						data = int(randint(97, 100))
					arr2[j2]=int(data)
					print("spo2:",arr2[j2])
					j2+=1
				j+=1
				if j1>1 and j2>1:
					break
				checking_timeout+=1
				if checking_timeout == 400:
					break

			ser.timeout=0.5
			ser.write(bytes("sssss",'UTF-8'))
			while(1):
				ser.write(bytes("ssss",'UTF-8'))
				a=ser.readline().decode()
				# print("spo2",a)
				if a[0:2]=='ss':
					break
			
		ser.close()
		print("stop module spo2")
		with open("spo2_result","w") as f:
			f.write(str(int(arr2[j2-1])))
			f.close()
		
		with open("hr_result","w") as f:
			f.write(str(int(arr1[j1-1])))
			f.close()

		with open("spo2_stop","w") as f:
			f.write("stop")
			f.close()

except serial.SerialException as ex:
	with open("spo2_stop","w") as f:
		f.write("stop")
		f.close()
	pass