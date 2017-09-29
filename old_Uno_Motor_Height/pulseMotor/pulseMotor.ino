const int phaA = 2;
const int phaB = 3;
const int MOVE = 13;
const int SPEED = 11;
const int trig = 10;
const int echo = 9;
const int input = 7;
const int Wait = 0;
const int StandBy = 1;
const int MoveUp = 2;
const int MoveDown = 3;

long pulse=0;
long distance = 0;
long distanceForMotor = 0;
int count = 0;
int Status = Wait;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Ready");
  pinMode(6, OUTPUT);
  pinMode(input, INPUT_PULLUP);
  
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

int Distance()
{
   unsigned long duration;
   int distance = 0;
  char inByte= ' ';
  
  if(Serial.available()){ // only send data back if data has been sent
  char inByte = Serial.read(); // read the incoming data
  distance = int(inByte);
  Serial.println(distance); // send the data back in a new line so that it is not all one long line
  }
  delay(100); // delay for 1/10 of a second
   return distance;
}

void _count(){
    if(digitalRead(phaA) == digitalRead(phaB)){
       pulse++;
       if(pulse >= distanceForMotor){ 
             digitalWrite(SPEED, LOW);
             pulse = 0; 
             digitalWrite(6, HIGH);
          }
      }else{
         pulse--;
         if(pulse <= -distanceForMotor){   
            digitalWrite(SPEED, LOW);  
            pulse = 0;
            digitalWrite(6, HIGH);
        }
     }
}

bool IsStable()
{
  int maxDistance, minDistance;
  int TestDistance[5];
  
  for (int i=0; i<1; i++)
  {
    TestDistance[i]=Distance();
    if(i < 4)delay(200);
    else delay(20);
  }
  
  maxDistance = TestDistance[0];
  minDistance = TestDistance[0];
  
  for (int i=1; i<2; i++)
  {
    if ( maxDistance < TestDistance[i])
    {
      maxDistance = TestDistance[i];
    }
    if ( minDistance > TestDistance[i])
    {
      minDistance = TestDistance[i];
    }
  }
  if(maxDistance <= minDistance + 3)
  {
    if (Status == Wait)
    {
      if ((maxDistance <= 50) && (maxDistance != 0))
      {
        distance = maxDistance - 1;
        return true;
      }
      else
      {
        return false;
      }
    }
    else if (Status == StandBy)
    {
      if (maxDistance > 50)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  else return false;
}
void loop() {
  // put your main code here, to run repeatedly:
  if (Status == Wait)
  {
    if(IsStable())
    {
      Status = MoveDown;
      distanceForMotor = distance*650/8;
      digitalWrite(6, LOW);
    }
  }
  if (Status == MoveDown)
  {
    Status = StandBy;
    digitalWrite(MOVE, LOW);
    analogWrite(SPEED, 150);
  }
  if (Status == StandBy)
  {
    if(IsStable())
    {
      Status = MoveUp;
      digitalWrite(6, LOW);
    }
  }
  if (Status == MoveUp)
  {
    Status = Wait;
    digitalWrite(MOVE, HIGH);
    analogWrite(SPEED, 220);
  }
}
