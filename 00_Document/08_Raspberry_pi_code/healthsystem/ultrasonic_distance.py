#Libraries
import RPi.GPIO as GPIO
import time
from array import array
#GPIO Mode (BOARD / BCM)
GPIO.setmode(GPIO.BCM)

#set GPIO Pins
GPIO_TRIGGER = 18
GPIO_ECHO = 24

#set GPIO direction (IN / OUT)
GPIO.setup(GPIO_TRIGGER, GPIO.OUT)
GPIO.setup(GPIO_ECHO, GPIO.IN)

def distance():
    # set Trigger to HIGH
    GPIO.output(GPIO_TRIGGER, True)

    # set Trigger after 0.01ms to LOW
    time.sleep(0.00001)
    GPIO.output(GPIO_TRIGGER, False)

    StartTime = time.time()
    StopTime = time.time()

    # save StartTime
    while GPIO.input(GPIO_ECHO) == 0:
        StartTime = time.time()

    # save time of arrival
    while GPIO.input(GPIO_ECHO) == 1:
        StopTime = time.time()

    # time difference between start and arrival
    TimeElapsed = StopTime - StartTime
    # multiply with the sonic speed (34300 cm/s)
    # and divide by 2, because there and back
    distance = (TimeElapsed * 34300) / 2

    return distance

if __name__ == '__main__':
	file=open("HeightResult.txt","w")
        result=[]
	for i in range(1,10):
		dist = distance()
		result.append(dist)
		file.write("%.1f\n" % dist)
		print ("Measured Distance = %.1f cm" % dist)
		time.sleep(1)
	maxfrequen=0
	position=5
	for i in range(len(result)-1):
		frequency=0
		for j in range(len(result)):
			if result[i]==result[j]:
				frequency=frequency+1
		if frequency>maxfrequen : 
			maxfrequen=frequency
			position=i
	file.write("%.f\n" % result[position])
	file.close()
	GPIO.cleanup()
