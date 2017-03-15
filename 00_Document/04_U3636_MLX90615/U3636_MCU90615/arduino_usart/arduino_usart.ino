/////////////////////
/*
GY-MCU90615----MINI
VCC----VCC
GND----GND
1:RX---TX,send A5 51 F6 to GY263
2:TX---RX
3:MINI_TX---FT232_RX
*/
//////////////////
unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
float object=0, ambient=0;

void setup() {
  Serial.begin(115200);  
  delay(1);    
  Serial.write(0XA5); 
  Serial.write(0X45);    
  Serial.write(0XEA);    
}

void loop() {
  unsigned char i=0, sum=0;
  
  if(sign)
  {   
     sign=0;
     for(i=0;i<8;i++)
      sum += Re_buf[i]; 
     if(sum == Re_buf[i])       
     {  	       
           object=(float)(Re_buf[4]<<8|Re_buf[5])/100;
           Serial.print("Object: ");
           Serial.print(object);  
           ambient=(float)(Re_buf[6]<<8|Re_buf[7])/100;
           Serial.print(" *C \tAmbient: ");
           Serial.print(ambient);  
           Serial.println(" *C");           
   }
  } 
  delay(1000);
}
void serialEvent() {
  while (Serial.available()) {   
    Re_buf[counter]=(unsigned char)Serial.read();
    if(counter==0&&Re_buf[0]!=0x5A) return;         
    counter++;       
    if(counter==9)               
    {    
       counter=0;                 
       sign=1;
    }      
  }
}
