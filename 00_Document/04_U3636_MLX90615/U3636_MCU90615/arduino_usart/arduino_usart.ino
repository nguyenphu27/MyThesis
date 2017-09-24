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
unsigned char Re_buf[8],counter=0;
unsigned char sign=0;
float object=0, ambient=0;

void setup() {
  Serial.begin(9600);  
  delay(1);    

}

void loop() {
  unsigned char i=0, sum=0;
  int sum1=0;
    Serial.write(0XA5); 
  Serial.write(0X15);    
  Serial.write(0XBA);    
//  if(sign)
//  {   
     sign=0;
     for(i=0;i<8;i++)
      {
        sum += Re_buf[i]; 
      }


     if(sum == Re_buf[i])       
     {  	     
      
//           Serial.print("sum: ", sum1);  
//           Serial.println(" rebuf i: ", (int)a); 
//           object=(float)(Re_buf[4]<<8|Re_buf[5])/100;
            object=(float)(Re_buf[8]);
//           Serial.print("Object: ");
//           Serial.println(object);  
           ambient=(float)(Re_buf[6]<<8|Re_buf[7])/100;
           Serial.print(" *C \tAmbient: ");
           Serial.print(ambient);  
           Serial.println(" *C");           
   }
//  } 
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

