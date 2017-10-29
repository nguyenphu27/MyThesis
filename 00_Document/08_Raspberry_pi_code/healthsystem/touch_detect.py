import RPi.GPIO as gpio
from time import sleep

gpio.setmode(gpio.BCM)
gpio.setup(4, gpio.IN, pull_up_down=gpio.PUD_DOWN)
gpio.output(4)
while(1):
	
