import RPi.GPIO as g
from time import sleep

g.setmode(g.BCM)
g.setup(4,g.IN,pull_up_down=g.PUD_DOWN)

state1=0
state2=0
while(1):
	if state1==state2:
		while(1):
			state1=g.input(4)
			if state1!= state2:
				print("PUSHED state 1!!!!")
				print("state 1:", state1)
				break
	sleep(2)
	state2=g.input(4)
	sleep(10)
	print("outside check")
