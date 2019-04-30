import RPi.GPIO as gpio
from time import sleep

gpio.setmode(gpio.BCM)
gpio.setwarnings(False)
gpio.setup(14, gpio.IN, pull_up_down=gpio.PUD_DOWN)
gpio.setup(15, gpio.IN, pull_up_down=gpio.PUD_DOWN)
gpio.setup(2, gpio.OUT, initial=gpio.LOW)
gpio.setup(3, gpio.OUT, initial=gpio.LOW)
count=0
while(1):
    a=gpio.input(14)
    b=gpio.input(15)
    if a and not b:
        gpio.output(2,gpio.HIGH)
        gpio.output(3,gpio.LOW)
    if b and not a:
        gpio.output(3,gpio.HIGH)
        gpio.output(2,gpio.LOW)
    if a==b:
        gpio.output(2,gpio.LOW)
        gpio.output(3,gpio.LOW)