#include "HoltzLib.h"
/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Updated by Jason A. Cox, February 18, 2023.
  Released into the public domain.
*/

DCMotor::DCMotor(uint8_t in1, uint8_t in2, uint8_t powerPin, boolean reversed)
{
  this->in1 = in1;
  this->in2 = in2;
  this->powerPin = powerPin;
  this->reversed = reversed;

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(powerPin, OUTPUT);
}

DCMotor::DCMotor(uint8_t in1, uint8_t in2, uint8_t powerPin)
{
  this->in1 = in1;
  this->in2 = in2;
  this->powerPin = powerPin;
  this->reversed = false;

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(powerPin, OUTPUT);
}

void DCMotor::drive(int power, boolean reverse)
{
  if ((reversed && reverse) || (!reversed && !reverse))
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  analogWrite(powerPin, power);
}

void DCMotor::drive(int power)
{
  drive(power, false);
}

UltrasonicSensor::UltrasonicSensor(uint8_t trig, uint8_t echo)
{
  this->trig = trig;
  this->echo = echo;

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

float UltrasonicSensor::getDistance()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  return (pulseIn(echo, HIGH) * 0.0343) / 2.0;
}
