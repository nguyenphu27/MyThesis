/*
 *  eHealth sensor platform for Arduino and Raspberry from Cooking-hacks.
 *
 *  Description: "The e-Health Sensor Shield allows Arduino and Raspberry Pi 
 *  users to perform biometric and medical applications by using 9 different 
 *  sensors: Pulse and Oxygen in Blood Sensor (SPO2), Airflow Sensor (Breathing),
 *  Body Temperature, Electrocardiogram Sensor (ECG), Glucometer, Galvanic Skin
 *  Response Sensor (GSR - Sweating), Blood Pressure (Sphygmomanometer) and 
 *  Patient Position (Accelerometer)."  
 *
 *  Explanation: This example shows the way to communicate with  
 *  the Arduino Wifi Demo Android app. 
 *
 *  Copyright (C) 2012 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version 2.0
 *  Author: Luis Martin & Ahmad Saad 
 */
 

#include <eHealth.h>
#include "Wire.h"

char recv[128];
uint8_t cont = 0;


void setup()
{ 
  Serial1.begin(9600);
  eHealth.initPositionSensor();
  
  delay(1000);
  
}

void loop()
{
  while (Serial1.available()>0) {}
  // Enters in command mode    
    Serial1.print("$$$"); check();  
  // Sets DHCP and TCP protocol
    Serial1.print("set ip dhcp 1\r"); check();
    Serial1.print("set ip protocol 1\r"); check();
  // Configures the way to join the network AP
    Serial1.print("set wlan join 0\r"); check();
    Serial1.print("join ANDROID\r"); check();
  
    Serial1.print("set i h 255.255.255.255\r"); delay(1000);
    
    Serial1.print("set i r 12345\r"); check();
    Serial1.print("set i l 2000\r"); check();
    Serial1.print("exit\r"); check();

    while(1){     
      
      //1. Read from eHealth. 
       int airFlow = eHealth.getAirFlow();     
       float temperature = eHealth.getTemperature();     
       float conductance = eHealth.getSkinConductance();   
       float resistance = eHealth.getSkinResistance();
       float conductanceVol = eHealth.getSkinConductanceVoltage();
       int BPM = 0;     
       int SPO2 = 0;     
       uint8_t pos = eHealth.getBodyPosition();     
       //int syst = eHealth.getSystolicPressure(1);
       //int diast = eHealth.getDiastolicPressure(1);     
       float ECG = eHealth.getECG();
       //uint8_t glucose = eHealth.glucoseDataVector[0].glucose;      

      //Data sensor must be sent in this order to mobile android application 
      Serial1.print(int(airFlow));     Serial1.print("#");
      Serial1.print(int(ECG));         Serial1.print("#");
      Serial1.print(int(0));           Serial1.print("#"); //Systolic is not implemented
      Serial1.print(int(0));           Serial1.print("#"); //Diastolic is not implemented 
      Serial1.print(int(0));           Serial1.print("#"); //Glucose is not implemented
      Serial1.print(int(temperature)); Serial1.print("#");
      Serial1.print(int(BPM));         Serial1.print("#");
      Serial1.print(int(SPO2));        Serial1.print("#");
      Serial1.print(int(conductance)); Serial1.print("#");
      Serial1.print(int(resistance));  Serial1.print("#");
      Serial1.print(int(airFlow));     Serial1.print("#");
      Serial1.print(int(pos));         Serial1.print("#");
      Serial1.print("\n");    
    
      // Reduce this delay for more data rate
      delay(250);  
  }
}

void check(){
  cont=0; delay(500);
  while (Serial1.available()>0)
  {
     recv[cont]=Serial1.read(); delay(10);
     cont++;
  }
  recv[cont]='\0';
  Serial.println(recv);
  Serial.flush(); delay(100);
}


