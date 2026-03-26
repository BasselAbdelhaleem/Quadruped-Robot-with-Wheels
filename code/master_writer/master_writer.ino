#include <Wire.h>

void setup()
{
  Serial.begin(9600);
  Wire.begin(); // join I2C bus
}

void loop()
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(Serial.parseInt());
  Wire.endTransmission();    // stop transmitting
}
