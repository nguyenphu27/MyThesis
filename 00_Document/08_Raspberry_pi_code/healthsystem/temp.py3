import serial
import time
from serial import tools
from serial.tools import list_ports
import sys 
import os
p=None
if os.path.exists("temp_port"):
	with open("temp_port","r") as f:
		p=f.read()
		f.close()
	os.remove("temp_port")
try:
	ser = serial.Serial(p,	baudrate = 9600,timeout=3)
	check2=0
	if ser.isOpen():
		j=0
		TO = [0]*7
		print("port",p,"is opened")
		ser.write(bytes("ID   ",'UTF-8'))
		while(1):
			check=ser.readline()
			if check==bytes("temp",'UTF-8'):
				print("start temp module")
				ser.write(bytes("start",'UTF-8'))
				while(1):
					ser.write(bytes("     ",'UTF-8'))
					a=ser.readline()
					data=a.decode()
					data=a
					data=data.rstrip()
					j+=1
					if j==4 and data!=bytes("     ",'UTF-8'):
						j=0
						print(data) 
						with open("temp_result","w") as f:
#							f.write(str(float(a)))
							f.close()
#							print("temp:",float(a))
						break
				ser.write(bytes("stop",'UTF-8'))
				ser.close()
				check2=1
				sys.exit(0)
			if check != bytes("temp",'UTF-8'):
				print("port {0} is not temp module".format(i))	
				break
except serial.SerialException as ex:
	pass
