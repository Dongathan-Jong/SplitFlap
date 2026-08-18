#include <Wire.h>

int numUnits = 1;
int currentUnit;
static byte value = 1;

void setup() {
  Serial.begin(115200);

  Wire.begin();

  currentUnit = numUnits + 7;

  delay(2000);
  

}

void loop() 
{
  value = 8;
  sendByte();
  delay(3000);
  value = 5;
  sendByte();
  delay(3000);
  value = 12;
  sendByte();
  delay(3000);
  value = 12;
  sendByte();
  delay(3000);
  value = 15;
  sendByte();
  delay(3000);
  

}

void sendByte()
{
  Wire.beginTransmission(currentUnit);
  Wire.write(value);
  Wire.endTransmission();
}
