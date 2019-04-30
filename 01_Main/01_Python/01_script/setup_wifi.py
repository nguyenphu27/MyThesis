""" Python script for setting up WiFi and send IP to my email"""
import os
# check ssid
remove_old_ssid=r'sudo rm /home/pi/Huy_script/ssid'
find_ssid = r'iwgetid -r > /home/pi/Huy_script/ssid'
dhclient = r'sudo dhclient -v'
cat = r'sudo cat /home/pi/Huy_script/staticip > /etc/dhcpcd.conf'
remove_old_dhcpcd = r'sudo cp /home/pi/Huy_script/dhcpcd.conf /etc/dhcpcd.conf'
reconfigure=r'sudo wpa_cli reconfigure'

try:
    os.system(remove_old_ssid)
except:
    pass
os.system(find_ssid)
print os.getcwd()
f = open('/home/pi/Huy_script/ssid', 'r')
ssid = f.read()

print ssid
os.system(remove_old_dhcpcd)

if 'Tam Hien' in ssid:
    os.system(cat)
else:
    os.system(dhclient)
    pass

os.system(reconfigure)
