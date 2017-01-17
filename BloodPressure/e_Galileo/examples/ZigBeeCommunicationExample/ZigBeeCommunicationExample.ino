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
 *  the Arduino using ZigBee protocol. 
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

//  Note :  The Xbee modules must be configured previously. 
//  See the next link http://www.cooking-hacks.com/index.php/documentation/tutorials/arduino-xbee-shield 

void setup()
{ 
  Serial1.begin(9600);
  delay(1000);
  
}

void loop()
{    
      
      //1. Read from eHealth. 
       int airFlow = eHealth.getAirFlow();     
       float temperature = eHealth.getTemperature();     
       float conductance = eHealth.getSkinConductance();   
       float resistance = eHealth.getSkinResistance();     
    

      Serial1.print(int(airFlow));     Serial.print("#");
      Serial1.print(temperature);      Serial.print("#");
      Serial1.print(conductance);      Serial.print("#");
      Serial1.print(int(resistance));  Serial.print("#");
      Serial1.print("\n");    
    
      // Reduce this delay for more data rate
      delay(250);   
}



