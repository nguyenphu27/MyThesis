#!/bin/bash

echo "turn on discovery mode\n" 
sudo echo -e 'discoverable on\nquit \n' | bluetoothctl
echo “run bluetooth server“
sudo python ./rfcomm-server.py
