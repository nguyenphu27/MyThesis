#include <Wire.h>
#include <Arduino.h>
#include "algorithm.h"
#include "max30102.h"

//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated.  Samples become 16-bit data.
uint16_t aun_ir_buffer[100]; //infrared LED sensor data  //edit 32bit if enough sram
uint16_t aun_red_buffer[100];  //red LED sensor data

int32_t n_ir_buffer_length; //data length
int32_t n_spo2;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

uint16_t temp_spo2;
int temp_object;
int temp_ambient;

int n_object;
int n_ambient;

int n=0;
int ack = 1;
int sclwire = 5;
int flag = 0;

//temperature
unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
float object=0, ambient=0;

// the setup routine runs once when you press reset:
void setup() {
  delay(1000);
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200); 
  pinMode(10, INPUT);  //pin D10 connects to the interrupt output pin of the MAX30102
  pinMode(7, INPUT_PULLUP);
  pinMode( 8, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite( 8, HIGH);
  digitalWrite(5, LOW);
  delay(100);
  
  maxim_max30102_reset(); //resets the MAX30102 //1
  delay(1000);
  maxim_max30102_read_reg(REG_INTR_STATUS_1,&uch_dummy);  //Reads/clears the interrupt status register

  Serial.println(F("Press any key to start conversion"));
  while(digitalRead(7) == HIGH){};
  Serial.println("measuring...");
  delay(1000);
  _measure();
  delay(2000);
}

void _measure(){
  
  maxim_max30102_init();  //initialize the MAX30102
  delay(1000);
  
  uint32_t un_min, un_max, un_prev_data, un_brightness;  //variables to calculate the on-board LED brightness that reflects the heartbeats
  int32_t i;
  float f_temp;
  
  un_brightness=0;
  un_min=0x3FFFF;
  un_max=0;
  
  n_ir_buffer_length=100;  //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for(i=0;i<n_ir_buffer_length;i++)
  {
    while(digitalRead(10)==1);  //wait until the interrupt pin asserts
    maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));  //read from MAX30102 FIFO
    
    if(un_min>aun_red_buffer[i])
      un_min=aun_red_buffer[i];  //update signal min
    if(un_max<aun_red_buffer[i])
      un_max=aun_red_buffer[i];  //update signal max
    Serial.print(F("red="));
    Serial.print(aun_red_buffer[i], DEC);
    Serial.print(F(", ir="));
    Serial.println(aun_ir_buffer[i], DEC);
  }
  un_prev_data=aun_red_buffer[i];
  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 

  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  
  while(1)
  {
    i=0;
    un_min=0x3FFFF;
    un_max=0;
    
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for(i=25;i<100;i++)
    {
      aun_red_buffer[i-25]=aun_red_buffer[i];
      aun_ir_buffer[i-25]=aun_ir_buffer[i];

      //update the signal min and max
      if(un_min>aun_red_buffer[i])
        un_min=aun_red_buffer[i];
      if(un_max<aun_red_buffer[i])
        un_max=aun_red_buffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for(i=75;i<100;i++)
    {
      un_prev_data=aun_red_buffer[i-1];
      while(digitalRead(10)==1);
      //digitalWrite(9, !digitalRead(9));
      maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));

      //calculate the brightness of the LED
      if(aun_red_buffer[i]>un_prev_data)
      {
        f_temp=aun_red_buffer[i]-un_prev_data;
        f_temp/=(un_max-un_min);
        f_temp*=MAX_BRIGHTNESS;
        f_temp=un_brightness-f_temp;
        if(f_temp<0)
          un_brightness=0;
        else
          un_brightness=(int)f_temp;
      }
      else
      {
        f_temp=un_prev_data-aun_red_buffer[i];
        f_temp/=(un_max-un_min);
        f_temp*=MAX_BRIGHTNESS;
        un_brightness+=(int)f_temp;
        if(un_brightness>MAX_BRIGHTNESS)
          un_brightness=MAX_BRIGHTNESS;
      }
  
      if(i == 90){
        if(ch_hr_valid != 0 && ch_spo2_valid != 0  ){ //&& object > 0 && ambient > 0 && object < 45 && ambient < 40                 
          Serial.println(n_heart_rate, DEC);
          Serial.println(n_spo2, DEC); 
          
          temp_spo2 = n_spo2;  
          n_spo2 = 0;
          n++;
          }
        }
      //send samples and calculation result to terminal program through UART     
    }
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
    delay(1000);
    if(n>=4){
        break;
      } 
  }

 //wire
 Serial.print("Waiting for request data...");
 Wire.begin(8);                // join i2c bus with address #8
 Wire.onRequest(requestEvent); // register event
 Wire.onReceive(receiveEvent);
 delay(2000);
 digitalWrite(5, HIGH); 
 delay(100);    
 Serial.write(0XA5); 
 Serial.write(0X45);    
 Serial.write(0XEA);
}

void requestEvent() {  
  byte n[] = {temp_spo2,n_object,temp_object,n_ambient,temp_ambient};
  Wire.write(n, 5);
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    }
  int ack = Wire.read();    // receive byte as an integer
  if(ack == 1){
      delay(500);
      maxim_max30102_reset(); //resets the MAX30102
      maxim_max30102_read_reg(REG_INTR_STATUS_1,&uch_dummy);  //Reads/clears the interrupt status register
      digitalWrite(5, LOW);
      delay(1000);
      digitalWrite( 8, LOW); 
  }
}

// the loop routine runs over and over again forever:
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
          ambient=(float)(Re_buf[6]<<8|Re_buf[7])/100;       

          Serial.println(object,1);
          Serial.println(ambient,1);
          
          temp_object = object * 10;
          n_object = temp_object /100;
          temp_object = temp_object % 100;
          
          temp_ambient = ambient * 10;
          n_ambient = temp_ambient / 100;
          temp_ambient = temp_ambient % 100;
        } 
      } 
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
