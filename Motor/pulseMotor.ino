const int phaA = 2;
const int phaB = 3;
const int MOVE = 13;
const int SPEED = 11;
const int trig = 10;
const int echo = 9;

long pulse=0;
long distanceForMotor = 0;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(6, OUTPUT);
  
  pinMode(phaA, INPUT_PULLUP);
  pinMode(phaB, INPUT_PULLUP);
  
  pinMode(MOVE, OUTPUT);
  pinMode(SPEED, OUTPUT);
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT_PULLUP);
  
  digitalWrite(phaA, LOW);
  digitalWrite(phaB, LOW);
  digitalWrite(6, HIGH);
  attachInterrupt(0, _count, CHANGE); 
}

void _count(){
    if(digitalRead(phaA) == digitalRead(phaB)){
       pulse++;
       if(pulse >= distanceForMotor){ 
            analogWrite(SPEED, LOW);
            digitalWrite(8,HIGH);
            delay(2);
            pulse = 0;  
            digitalWrite(8,LOW);
            digitalWrite(MOVE, HIGH);
            analogWrite(SPEED, 150); 
          }
      }else{
         pulse--;
         if(pulse <= -distanceForMotor){   
            analogWrite(SPEED, LOW);
            digitalWrite(8,HIGH);    
            delay(2);
            pulse = 0;
            //digitalWrite(MOVE, LOW);
            analogWrite(SPEED, LOW);delay(2); 
            digitalWrite(6, HIGH);
        }
     }
}
  
void loop() {
  // put your main code here, to run repeatedly:
   //Serial.println(pulse);

   unsigned long duration;
   int distance;
   
   digitalWrite(trig,0);   // tắt chân trig
   delayMicroseconds(2);
   digitalWrite(trig,1);   // phát xung từ chân trig
   delayMicroseconds(5);   // xung có độ dài 5 microSeconds
   digitalWrite(trig,0);   // tắt chân trig
   
   duration = pulseIn(echo,HIGH);  
   distance = int(duration/2/29.412);
   
   Serial.print(distance);//Serial.print(210 - distance);
   Serial.println("cm");
   delay(300);

      if(distance < 80 && distance != 0){   
        count++;
        if(count >= 3){
            count = 0;
            int mround = distance/8;
            distanceForMotor = mround*650;//Serial.println(distanceForMotor);
            delay(2000);
            digitalWrite(6, LOW);
            digitalWrite(MOVE, LOW);
            analogWrite(SPEED, 150);
         }
   }
}
