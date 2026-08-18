#include <Wire.h>

#define slaveAddr 8
#define magnetPin A6
#define enablePin 5
#define directionPin 4
#define stepPin 3 

int offset = 3150;
int currentPos;
int homingOffset;
bool homing;

byte lastValue;
byte payload;

void setup() {
  Serial.begin(9600);
  Wire.begin(slaveAddr);
  Wire.onReceive(receiveData);
  homing = true;
  pinMode(enablePin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  digitalWrite(directionPin, LOW);
  digitalWrite(7, HIGH); // MS2
  digitalWrite(8, HIGH); // MS1  
  digitalWrite(9, HIGH); // MS3

  delay(1000);
  digitalWrite(enablePin, LOW);

}

void loop() 
{
  while(homing)
  {
    homingSeq();
  }
  
}

void homingSeq()
{
  Serial.println("homing");
  while(homing)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);
    if(analogRead(magnetPin) < 100)
    {
      homing = false;
      Serial.println("homing stopped");
      currentPos = 0;
    }
  }
  for(int i = 0; i < offset; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);
  }
}

void receiveData()
{
  while(Wire.available())
  {
    payload = Wire.read();
    Serial.println(payload);
    updateBoard();
    lastValue = payload;
  }
}

void updateBoard()
{
  
  if(int(payload) == 0)
  {
    homing = true;
  }
  else if(lastValue == payload)
  {
    for(int i = 0; i < 3200; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(5);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(600);
    }
  }
  else
  {
    int aimPos = int(payload) * 71;
    int motorDiff = aimPos - currentPos;
    int motorMove;
    if(motorDiff < 0)
    {
      motorMove = 3200 + motorDiff;
    }
    else
    {
      motorMove = motorDiff;
    }
    for(int i = 0; i < motorMove; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(5);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(600);
    }

    currentPos = aimPos;
  }
  
}



