import serial
from time import sleep
from serial import tools
from serial.tools import list_ports

def check_port():
	for i in range(4):
		try:
			ser = serial.Serial("/dev/ttyUSB{0}".format(i),
				9600, timeout=1)
			if ser.isOpen():
				print "port",i,"is opened"
				ser.write(bytes("ID   ").encode('UTF-8'))
				while(1):
					a=ser.readline().decode()
					print "get",a,"from port",i
					if a!=bytes('').encode('UTF-8'):
						with open(a+'_port',"w") as f:
							f.write(str("/dev/ttyUSB{0}".format(i)))
						break
#				ser.write(bytes("stop ").encode('UTF-8'))
#				a=ser.readline().decode()
				ser.close()
		except serial.SerialException as ex:
			pass
