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
 *  the Arduino Wifi Demo iPhone app. 
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
 *  Author: Ahmad Saad & Luis Martin
 */
 

#include <eHealth.h>
#include "Wire.h"

char recv[256];
uint8_t cont = 0;


void setup(){ 

  //eHealth.readGlucometer();
  Serial1.begin(9600);

} 

void loop(){ 

  Serial1.print("$$$"); 
  check();  

  // 1. Configure the transport protocol (UDP, TCP, FTP, HTTP...)  
  Serial1.print("set i p 3\r");  check();   
  // 2. Configure the way the modules will resolve the IP address. 
  Serial1.print("set i d 2\r");  check();   
  // 3. Configure how to connect the AP. 
  Serial1.print("set w a 6\r");  check();   
  // 3.1 Sets the name of the ADhoc network.  
  Serial1.print("set w s iPHONE_ADHOC\r");  check();   
  // 3.2 Sets the channel of the ADhoc network     
  Serial1.print("set w c 6\r");  check();

  Serial1.print("set w j 4\r");  check();
  Serial1.print("save\r");  check();
  Serial1.print("exit\r");  check();
  Serial1.print("$$$");  check();
  // Configures UDP host and ports.
  Serial1.print("set i h 255.255.255.255\r");  check();
  Serial1.print("set i r 12345\r");  check();
  Serial1.print("set i l 2000\r");  check();
  // Checks if everything is Ok, even if It's correctly connected.

  // Exits from command mode, and then the UDP messages 
  // can be sent and received.
  Serial1.print("exit\r");
  check();

  delay(1000);  


  while(1){     

    //1. Read from eHealth. 
    int airFlow = eHealth.getAirFlow();     
    float temperature = eHealth.getTemperature();     
    float conductance = eHealth.getSkinConductance();   
    float resistance = eHealth.getSkinResistance();
    float conductanceVol = eHealth.getSkinConductanceVoltage();
    int BPM = 0;     
    int SPO2 = 0;     
    uint8_t pos = 0;      
    //int syst = eHealth.getSystolicPressure(1);
    //int diast = eHealth.getDiastolicPressure(1);         
    float ECG = eHealth.getECG();
    //uint8_t glucose = eHealth.glucoseDataVector[0].glucose;

    // 2. Send data to the Android smartphone    
    Serial1.print(int(airFlow));         Serial1.print("#");
    Serial1.print(ECG);                  Serial1.print("#");
    Serial1.print(temperature);          Serial1.print("#");
    Serial1.print(int(BPM));             Serial1.print("#");
    Serial1.print(int(SPO2));            Serial1.print("#");
    Serial1.print(conductance);          Serial1.print("#");
    Serial1.print(int(resistance));      Serial1.print("#");
    Serial1.print(int(airFlow));         Serial1.print("#");
    Serial1.print(0);                    Serial1.print("#");  //Systolic is not implemented
    Serial1.print(0);                    Serial1.print("#");  //Diastolic is not implemented 
    Serial1.print(int(0));               Serial1.print("#");  //Glucose is not implemented
    Serial1.print(int(pos));             Serial1.print("#");
    Serial1.print("\n");    
    
    // Reduce this delay for more data rate
    delay(100);  
  }

}

//=========================================================================

void check(){
  cont=0; 
  delay(500);
  while (Serial1.available()>0)
  {
    recv[cont]=Serial1.read(); 
    delay(10);
    cont++;
  }
  recv[cont]='\0';
  Serial.println(recv);
  Serial1.flush(); 
  delay(100);
}



