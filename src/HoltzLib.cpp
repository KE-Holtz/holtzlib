#include "HoltzLib.h"
/*
  HoltzLib.cpp - A library for simple abstractions for common components.
  Created and maintained by Kyle Holtz for EDD
  Released into the public domain.
*/

/**
 * Constructs a DCMotor using the L298N motor controller.
 * @param in1 The arduino pin connected to in1 or in3 on the motor controller.
 * @param in2 The arduino pin connected to in2 or in4 on the motor controller.
 * @param in3 The arduino pin connected to ENA or ENB on the motor controller. Must be PWM.
 * @param reversed If true, the motor will run backwards. Useful if two motors are facing opposite directions.
 */
DCMotor::DCMotor(uint8_t in1, uint8_t in2, uint8_t powerPin, boolean reversed)
    : in1(in1), in2(in2), powerPin(powerPin), reversed(reversed), currentDirectionReversed(false)
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(powerPin, OUTPUT);
}

/**
 * Constructs a DCMotor using the L298N motor controller.
 * @param in1 The arduino pin connected to in1 or in3 on the motor controller.
 * @param in2 The arduino pin connected to in2 or in4 on the motor controller.
 * @param powerPin The arduino pin connected to ENA or ENB on the motor controller. Must be PWM.
 */
DCMotor::DCMotor(uint8_t in1, uint8_t in2, uint8_t powerPin)
    : DCMotor(in1, in2, powerPin, false)
{
}

/**
 * Drive the the motor with a given power and direction.
 * @param power The power the motor will be driven at. Must be in the range [0,255]
 * @param reverse If true, the motor will be driven in reverse. If false, it will drive forward.
 */
void DCMotor::drive(int power, boolean reverse)
{
  boolean trueReverse = reverse != reversed;
  if (trueReverse)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }

  currentDirectionReversed = trueReverse;
  analogWrite(powerPin, power);
}

/**
 * Drive the the motor with a given power.
 * @param power The power the motor will be driven at. Must be in the range [0,255]
 */
void DCMotor::drive(int power)
{
  drive(power, false);
}

/**
 * Stops the motor.
 */
void DCMotor::stop()
{
  drive(0, false);
}

/**
 * Returns the direction of the motor based on it's last commanded speed
 * and its configuration. If the last commanded speed was zero,
 * this method will return the direction of the motor as if it a speed.
 * @return True if the motor is running in reverse, otherwise false.
 */
boolean DCMotor::isReversed()
{
  return currentDirectionReversed;
}

/**
 * Constructs an Ultrasonic Sensor
 * @param trig The pin on the arduino connected to the trig pin of the sensor.
 * @param echo The pin on the arduino connected to the echo pin of the sensor.
 */
UltrasonicSensor::UltrasonicSensor(uint8_t trig, uint8_t echo)
    : trig(trig), echo(echo)
{
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

/**
 * Get the distance reading of the ultrasonic sensor in centimeters.
 * @return distance in centimeters.
 */
float UltrasonicSensor::getDistance()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  return (pulseIn(echo, HIGH) * 0.0343) / 2.0;
}

Interrupter* Interrupter::instances[2] = { nullptr, nullptr};

Interrupter::Interrupter(uint8_t pin)
:pin(pin), interruptNum(digitalPinToInterrupt(pin))
{
}


boolean Interrupter::attach(int mode){
  if (interruptNum == NOT_AN_INTERRUPT)
  {
    return false;
  }
  
  instances[interruptNum] = this;

  switch (interruptNum)
  {
  case 0:
  attachInterrupt(0, interruptRoutine0, mode);
    break;
  case 1:
  attachInterrupt(1, interruptRoutine1, mode);
    break;
  }

  return true;
}

void Interrupter::detatch(){
  detachInterrupt(interruptNum);
  instances[interruptNum] = nullptr;
}




RotaryEncoder::RotaryEncoder(uint8_t out, int slots, DCMotor *motor)
    :Interrupter(out), slots(slots), motor(motor), slotsPassed(0)
{
}

void RotaryEncoder::onInterrupt()
{
  if (motor != nullptr)
  {
    if (motor->isReversed())
    {
      slotsPassed--;
    }
    else
    {
      slotsPassed++;
    }
  }
  else
  {
    slotsPassed++;
  }
}

boolean RotaryEncoder::begin()
{
  return attach();
}

void RotaryEncoder::reset()
{
  noInterrupts();
  slotsPassed = 0;
  interrupts();
}

float RotaryEncoder::getDegrees()
{
  return getRotations() * 360;
}

float RotaryEncoder::getRotations()
{
  int slotsPassedCopy;
  noInterrupts();
  slotsPassedCopy = slotsPassed;
  interrupts();
  return ((float)slotsPassedCopy) / slots;
}

float RotaryEncoder::getDistance(float wheelDiameter)
{
  return getRotations() * PI * wheelDiameter;
}