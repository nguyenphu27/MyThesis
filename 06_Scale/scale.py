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
ser = serial.Serial(serial_port, baud_rate)
count = 0
delay_sample = 0

while True:
    line = ser.readline();
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line);
    delay_sample += 1
    if delay_sample==2:
        start_file = open(make_this_file, "w+")
        start_file.close()

    if '0.0' not in line:
        count +=1
        result.append(line)
        if count==3:
            output_file = open(write_to_file_path, "w+");
            output_file.write(result[1]);
            output_file.close()
            break
