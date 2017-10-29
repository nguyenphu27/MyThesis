##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial

serial_port = '/dev/ttyACM0';
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "scale_result";
make_this_file = "scale_start"
result=list()
#try:
ser = serial.Serial(serial_port, baud_rate)
#except serial.serialutil.SerialException:
	
count = 0
delay_sample = 0

while True:
#    try:
#        ser = serial.Serial(serial_port, baud_rate)
#    except serial.serialutil.SerialException:
#        break
    line = ser.readline()
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line)
    if delay_sample==2:
        start_file = open(make_this_file, "w+")
        start_file.close()
        delay_sample += 1
    elif delay_sample<2:
        delay_sample += 1

    if '0.0' not in line:
        for i in range (20):
            line = ser.readline()
            line = line.decode("utf-8") #ser.readline returns a binary, convert to string
            print(line)
            result.append(float(line[0:len(line)-7]))
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

ser.close() #close serial
