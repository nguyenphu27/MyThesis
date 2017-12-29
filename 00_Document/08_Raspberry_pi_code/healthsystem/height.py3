import serial
import time
from serial import tools
from serial.tools import list_ports
import sys 
import os

p=None
p2='/dev/ttyACM0'

if os.path.exists("height_port"):
	with open("height_port","r") as f:
		p=f.read()
		f.close()
	os.remove("height_port")
connect = 1

if os.path.exists("scale_port"):
	with open("scale_port","r") as f:
		check_p=f.read()
		f.close()
		if 'ttyACM0' in check_p:
			p2 = '/dev/ttyACM1'

checking_timeout = 0

if os.path.exists("height_stop"):
	os.remove("height_stop")

if os.path.exists("height_result"):
	os.remove("height_result")

try: 
	ser_motor = serial.Serial(p2, baudrate = 9600,timeout=1)
except serial.SerialException as ex:
	print("cannot connect to motor of temp module")
	connect=0

try:
	ser = serial.Serial(p, baudrate = 9600, timeout=1)
	if ser.isOpen():
		j=0
		TO = [0]*7
		#print("port",p,"is opened")
		ser.write(bytes("ID   ",'UTF-8'))
		while(1):
			check=ser.readline()
			if check==bytes("height",'UTF-8'):
				print("start height module")
				while(1):
					ser.write(bytes("start",'UTF-8'))
					a=ser.readline().rstrip().decode()
					# print("height:",a)
					if 'start' in a:
						checking_timeout=0
						break
					checking_timeout+=1
					if checking_timeout == 20:
						break

				if checking_timeout==0: #timeout safe switch for module
					while(1):
						ser.write(bytes("     ",'UTF-8'))
						a=ser.readline()
						data=a.decode()
						# print(data)
						j+=1
						checking_timeout+=1
						if checking_timeout == 60:
							break
						if data != '' and int(data) > 0 and int(data) < 60:
							j=0
							checking_timeout = 0
							with open("height_result","w") as f:
								print("height:",204-int(data))
								f.write(str(204-int(data)))
								f.close()

							if connect:
								checking_timeout=0
								data=str(int(a))+'!'
								for k in range(3):
									#print("send distance to temp motor")
									ser_motor.write(bytes(data,'UTF-8'))
									a=ser_motor.readline().rstrip().decode()
								with open("go_down","w") as f:
									f.write("aa")
									f.close
								while(1):
									ser.write(bytes("     ",'UTF-8'))
									a=ser.readline().rstrip().decode()
									data=a
									if data != '':
										if int(data)>60:
											data=str(int(a))+'!'
											for k in range(4):
												ser_motor.write(bytes(data,'UTF-8'))
												a=ser_motor.readline().rstrip().decode()
											break
									# checking_timeout+=1
									# if checking_timeout == 20:
									# 	break
							break
						
					while(1):
						ser.write(bytes("stop",'UTF-8'))
						a=ser.readline().rstrip().decode()
						if "stop" in a:
							break

				print("stop module height")
				with open("height_stop","w") as f:
					f.write("stop")
					f.close
				ser.close()
				sys.exit(0)

except serial.SerialException as ex:
	print("cannot connect height module")
	with open("height_stop","w") as f:
		f.write("stop")
		f.close
	pass