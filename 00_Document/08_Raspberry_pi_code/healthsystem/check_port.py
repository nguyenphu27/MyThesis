import serial
from time import sleep
from serial import tools
from serial.tools import list_ports
import os

def check_port():
	for i in range(4):
		try:
			ser = serial.Serial("/dev/ttyUSB{0}".format(i),
				9600, timeout=1)
			if ser.isOpen():
				ser.write(bytes("ID   ").encode('UTF-8'))
				while(1):
					a=ser.readline().decode()
					print "module",a,"is connected to port USB",i
					if a!=bytes('').encode('UTF-8'):
						with open(a+'_port',"w") as f:
							f.write(str("/dev/ttyUSB{0}".format(i)))
						break
		except serial.SerialException as ex:
			pass

	for i in range(4):
		try:
			ser = serial.Serial("/dev/ttyACM{0}".format(i),
				9600, timeout=0.5)
			if ser.isOpen():
				for j in range(7):
					ser.write(bytes("ID").encode('UTF-8'))
					a=ser.readline().rstrip().decode()
					sleep(1)
					if a=='scale':
						os.system(str('stty -F /dev/ttyACM{}'.format(i))+' -hupcl')
						print "module",a,"is connected to port ACM",i
						with open(a+'_port',"w") as f:
							f.write(str("/dev/ttyACM{0}".format(i)))
						break
		except serial.SerialException as ex:
			pass