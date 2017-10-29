from bluetooth import *
import json 

def bluetoothserver():
	port = 1

	server_sock=BluetoothSocket( RFCOMM )

	server_sock.bind(("",port))
	server_sock.listen(1)
	print "port %d" % port
	
	uuid = "00001101-0000-1000-8000-00805F9B34FB"
	advertise_service( server_sock, "Pi Bluetooth Service",uuid)

	client_sock, client_info = server_sock.accept()
	print 'Accepted connection from ', client_info
	string = str(client_info)
	with open("bluetooth_result","w") as f:
		f.write(string[2:19])
		f.close
	try:
	        while True:
	                data = client_sock.recv(4096)
	                if len(data) == 0:
	                        print("no data")
				return
	                elif data == 'disconnected':
	                        print("received [%s]" % data)
#                        with open("bluetooth_result","a") as f:
#                                f.write(data)
	                        client_sock.close()
	                        server_sock.close()
	                        print("all done")  
				return      
	                else:
	                        print("received [%s]" % data)                    
#                        with open("bluetooth_result","a") as f:
#                                f.write(data)

	except IOError:
        	print("disconnected")
		return
	except KeyboardInterrupt:
	        print("keyboard interrupted - close connection ")
	        client_sock.close()
	        server_sock.close()
	        print("disconnected")
		return

bluetoothserver()
