""" Python script for setting up WiFi and send IP to my email"""
import os
import smtplib
from time import sleep
def send_email(user, pwd, recipient, subject):

    gmail_user = "raspberry.pi.mtcl2013@gmail.com"
    gmail_pwd = "mtcl2013"
    FROM = user
    TO = recipient if type(recipient) is list else [recipient]
    SUBJECT = subject
    for i in xrange(5):
        os.system(r'sudo rm /home/pi/Huy_script/ip')
        os.system(r'ifconfig wlan0 | grep "inet addr" > /home/pi/Huy_script/ip')
        f = open('/home/pi/Huy_script/ip','r')
        ip = f.read()
        body = "Dear Professor Huy,\nMy IP is: " + ip + "\nBest regards,\nPi"
        TEXT = body
        f.close()
        # Prepare actual message
        message = """From: %s\nTo: %s\nSubject: %s\n\n%s
        """ % (FROM, ", ".join(TO), SUBJECT, TEXT)
        try:
            server = smtplib.SMTP("smtp.gmail.com", 587)
            server.ehlo()
            server.starttls()
            server.login(gmail_user, gmail_pwd)
            server.sendmail(FROM, TO, message)
            server.close()
            break
        except Exception as e:
            sleep(4)
    
# send email
s = open('/home/pi/Huy_script/ssid','r')
ssid = s.read()
if 'Tam Hien' in ssid:
    pass
else:
    send_email("pi_01","","huyvu7495@gmail.com","[Raspberry Pi]")
