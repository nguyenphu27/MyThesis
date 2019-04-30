#include <Wire.h>
#include <eHealth.h>
#include <SPI.h>
#include <WiFi.h>
#include <LiquidCrystal.h>

#define power 7 //HIGH in some second blood pressure will on or off 
#define pump  2 //when pump is HIGH from LOW interrupt active
#define uart  4 //LOW to mesure HIGH to get data

char ssid[] = "TP-LINK_E9C802"; 
char server[] = "www.uhealth123.96.lt";

int status = WL_IDLE_STATUS;

WiFiClient client;

int flagWifi = 0;

String temp = "";
String bpm = "";
String temperature = "";

uint8_t systolic;
uint8_t diastolic;
uint8_t pulse;

int count=0;

byte spo2, n_object, tmp_object, n_ambient, tmp_ambient;
float object, ambient;
byte ack=1;

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup_WiFi(){
  Serial.begin(115200);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    while(true);
  }
  String fv = WiFi.firmwareVersion();
  
  if( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");
    
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid);
    
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  lcd.clear();
  lcd.print("Connected wifi");
}

void setup() {
  //
  Serial.begin(115200);
  lcd.begin(16, 2);
  pinMode(power, OUTPUT);
  pinMode(uart, OUTPUT);
  pinMode(2, INPUT);
  delay(1000);
  digitalWrite(uart, LOW);

  Serial.println("Press any key to start!");
  //lcd.setCursor(0,1);
  lcd.clear();
  lcd.print("Press button!");
  attachInterrupt(3, _measure, FALLING);
  while(1);
}

void _measure(){
  delay(1000);
  Serial.println("Measuring..");
  lcd.clear();
  lcd.print("Measuring ... ");
  getMessure();
  delay(1000);
  lcd.clear();
  lcd.print("get data..");
  getBloodData();

  //lcd.clear();
  getDataSpO2();
  delay(500);
  
  //Serial.begin(115200);
  //
  Serial.print("Heart rate: ");
  Serial.print(pulse);
  Serial.print(" *bpm\tSystolic: ");
  Serial.print(systolic);
  Serial.print(" mmHg\tDiastolic: ");
  Serial.print(diastolic);
  Serial.print(" mmHg\tSpO2: ");
  Serial.print(spo2);
  Serial.print(" %\tBody: ");
  Serial.print(object);
  Serial.print(" *C\tAmbient: ");
  Serial.print(ambient);
  Serial.println(" *C");
  
  delay(1000);
  Serial.println("Waiting for upload data to server...");
  
  //
  if (client.connect(server, 80)) {
      Serial.println("connected server");
      // Make a HTTP request:
      client.print("GET /uhealth/send/upData.php?nhiptim=");
      client.print(pulse);
      client.print("&nhietdo=");
      client.print(object);
      client.print("&spo2=");
      client.print(spo2);
      client.print("&tamthu=");
      client.print(systolic);
      client.print("&tamtruong=");
      client.print(diastolic);
      client.print("&moitruong=");
      client.print(ambient);
      
      client.println(" HTTP/1.1");
      client.println("Host: www.uhealth123.96.lt");
      client.println("Connection: close");
      client.println();
      } else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
      lcd.clear();
      lcd.print("upload data failed.");
      }
  Serial.println("upload success!");
  lcd.clear();
  lcd.print("sending data..");
  delay(500);
  lcd.clear();
  lcd.print("Completed!");
  delay(2000);
  system("./opt/cln/galileo/galileo_sketch_reset_script.sh"); 
}
  
void getMessure(){
  delay(1000);
  digitalWrite(uart, LOW);
  //turn on
  digitalWrite(power, HIGH);
  delay(2000);
  digitalWrite(power, LOW);
  delay(10000);
  while(digitalRead(pump) == 0){
    if(flagWifi == 0){
        Serial.println("Setting wifi...");
        lcd.clear();
        lcd.print("Setting wifi...");
        setup_WiFi();
        flagWifi = 1;
      }
  }
  delay(1000);
  //turn off
  digitalWrite(power, HIGH);
  delay(1000);
  digitalWrite(power, LOW);
}

void getBloodData(){
  digitalWrite(uart, HIGH); //set uart = 1 to get data  
  delay(3000);Serial.println("pass1");
  eHealth.readBloodPressureSensor();Serial.println("pass2");
  delay(100); 
  uint8_t numberOfData = eHealth.getBloodPressureLength(); 
  delay(100);
  systolic = 30+eHealth.bloodPressureDataVector[numberOfData-1].systolic;
  diastolic = eHealth.bloodPressureDataVector[numberOfData-1].diastolic;
  pulse = eHealth.bloodPressureDataVector[numberOfData-1].pulse;

  delay(1000);
  digitalWrite(uart, LOW);
}

void getDataSpO2(){
  Wire.begin();  
  delay(1000);
  Wire.requestFrom(8, 5);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    spo2 = Wire.read(); // receive a byte as character
    n_object = Wire.read(); // receive a byte as character
    tmp_object = Wire.read(); // receive a byte as character
    n_ambient = Wire.read();  // receive a byte as character
    tmp_ambient = Wire.read(); // receive a byte as character
   }
   
    object = n_object*100 + tmp_object;
    ambient = n_ambient*100 + tmp_ambient;

    object = object / 10.0;
    ambient = ambient / 10.0;

    delay(1000);
    Serial.println("Request data OK! Reset spo2 sensor.");
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(ack);        // sends five bytes
    Wire.endTransmission();    // stop transmitting
    delay(1000);
  }
  
void loop() {}

