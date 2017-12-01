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

if os.path.exists("temp_stop"):
	os.remove("temp_stop")
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
				while(1):
					ser.write(bytes("start",'UTF-8'))
					a=ser.readline().rstrip().decode()
					if a == 'start':
						break
				while(1):
					ser.write(bytes("     ",'UTF-8'))
					a=ser.readline().rstrip().decode()
					data=a
					if data!=' ':
						print(data) 
						with open("temp_result","w") as f:
							f.write(str(a))
							f.close()
							print("temp:",float(a))
						break
				while(1):
					ser.write(bytes("stop",'UTF-8'))
					a=ser.readline().rstrip().decode()
					if "stop" in a:
						break
				print("stop module temp")
				with open("temp_stop","w") as f:
					f.write("stop")
					f.close
				ser.close()
				check2=1
				sys.exit(0)
except serial.SerialException as ex:
	pass
