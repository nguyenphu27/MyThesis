import serial
import time
from serial import tools
from serial.tools import list_ports
import sys 
import os
p=None

if os.path.exists("height_port"):
	with open("height_port","r") as f:
		p=f.read()
		f.close()
	os.remove("height_port")
connect = 1

try: 
	ser_motor = serial.Serial('/dev/ttyACM0', baudrate = 9600,timeout=1)
except serial.SerialException as ex:
	print("cannot connect to motor of temp module")
	connect=0

try:
	ser = serial.Serial(p, baudrate = 9600,timeout=1)
	check2=0
	if ser.isOpen():
		j=0
		TO = [0]*7
		print("port",p,"is opened")
		ser.write(bytes("ID   ",'UTF-8'))
		while(1):
			check=ser.readline()
			if check==bytes("height",'UTF-8'):
				print("start height module")
				ser.write(bytes("start",'UTF-8'))
				while(1):
					ser.write(bytes("     ",'UTF-8'))
					a=ser.readline()
					data=a.decode()
					print(data)
					j+=1
					if data != '':
						j=0
						with open("height_result","w") as f:
							f.write(str(200-int(data)))
							f.close()
						if connect:
							data=str(int(a))+'!'
							for k in range(3):
								print("send distance to temp motor")
								ser_motor.write(bytes(data,'UTF-8'))
								a=ser_motor.readline()
							while(1):
								print("wait for response")
								ser.write(bytes("     ",'UTF-8'))
								a=ser.readline().decode()
								data=a
								print("distance after finished:",data)
								if data!=bytes("     ",'UTF-8'):
									if int(data)>50:
										for k in range(3):
											ser_motor.write(bytes(data,'UTF-8'))
											a=ser_motor.readline()
										break
						break
				ser.write(bytes("stop",'UTF-8'))
				ser.close()
				check2=1
				sys.exit(0)
			if check != bytes("height",'UTF-8'):
				print("port",p,"is not height module")	
				break
except serial.SerialException as ex:
	print("cannot connect height module")
	pass
