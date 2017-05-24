##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial

serial_port = '/dev/ttyACM0';
baud_rate = 9600; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "scale_result";
result=list()
output_file = open(write_to_file_path, "w+");
ser = serial.Serial(serial_port, baud_rate)
count = 0
while True:
    line = ser.readline();
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line);
    if '0.0' not in line:
        count +=1
        result.append(line)
        if count==4:
            output_file.write(result[2]);
            break
