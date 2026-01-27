#ifndef HOLTZLIB_H
#define HOLTZLIB_H

#include <Arduino.h>

class DCMotor
{
private:
  uint8_t in1;
  uint8_t in2;
  uint8_t powerPin;
  boolean reversed;

public:
  DCMotor(uint8_t in1, uint8_t in2, uint8_t powerPin, boolean reversed);
  DCMotor(uint8_t in1, uint8_t in2, uint8_t powerPin);

  void drive(int power, boolean reverse);
  void drive(int power);
};

class UltrasonicSensor
{
private:
  uint8_t trig;
  uint8_t echo;

public:
  UltrasonicSensor(uint8_t trig, uint8_t echo);
  float getDistance();
};

#endif
