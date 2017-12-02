##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial

serial_port = None
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "scale_result";
make_this_file = "scale_start"
result=list()

if os.path.exists("scale_port"):
	with open("scale_port","r") as f:
		serial_port=f.read()
		f.close()

try:
    ser = serial.Serial(serial_port, baud_rate, timeout=0.5)
    count = 0
    delay_sample = 0
    if ser.isOpen():
        while(1):
            ser.write(bytes("start").encode('UTF-8'))
            a=ser.readline().rstrip().decode()
            sleep(1)
            if a=='start':
                print("start module scale")
                break
       
        while True:
            line = ser.readline()
            line = line.decode("utf-8") #ser.readline returns a binary, convert to string
            print(line)
            sleep(1)
            
            if delay_sample==2:
                start_file = open(make_this_file, "w+")
                start_file.close()
                delay_sample += 1
            elif delay_sample<2:
                delay_sample += 1

            if '0.0' not in line:
                for i in range (20):
                    line = ser.readline().rstrip().decode()
                    print(line)
                    result.append(float(line))
                    if i>=2:
                        diff_0 = (result[i-1] - result[i]) / (result[i])
                        diff_1 = (result[i-2] - result[i]) / (result[i])
                        #if result[i] == result[i-1] and result[i] == result[i-2]:
                        if abs(diff_0)<0.05 and abs(diff_1)<0.05:
                            break
                output_file = open(write_to_file_path, "w+")
                output_file.write(str(result[i]) + ' kgs')
                output_file.close()
                break
        
        while(1):
            ser.write(bytes("stop").encode('UTF-8'))
            a=ser.readline().rstrip().decode()
            sleep(1)
            if a=='stop':
                print("stop module scale")
                break
        
        with open("scale_stop","w") as f:
					f.write("stop")
					f.close
    ser.close() #close serial
except serial.serialutil.SerialException as er:
    print(er)