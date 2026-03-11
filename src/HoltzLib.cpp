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

/**
 * A generic class that represents any system with an interrupt.
 * This allows classes to have non-static methods be called by interrupts.
 * @param pin The pin that the interrupt will be on. Only some pins can have interrupts attached to them.
 * On the Arduino UNO, these are pins 2 and 3
 */
Interrupter::Interrupter(uint8_t pin)
:pin(pin), interruptNum(digitalPinToInterrupt(pin))
{
}

/**
 * Attaches the interrupt
 * @param mode The mode of the interrupt
 */
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

/**
 * Detaches the interrupt
 */
void Interrupter::detatch(){
  detachInterrupt(interruptNum);
  instances[interruptNum] = nullptr;
}



/**
 * Creates an optical rotary encoder.
 * @param out The output pin of the encoder. MUST be a pin with an interrupt (2 or 3 on Arduino UNO)
 * @param slots The number of slots in the wheel. Default 20
 * @param motor The motor that the encoder is attached to. (Optional)
 */
RotaryEncoder::RotaryEncoder(uint8_t out, int slots, DCMotor *motor)
    :Interrupter(out), slots(slots), motor(motor), slotsPassed(0)
{
}

RotaryEncoder::RotaryEncoder(uint8_t out, DCMotor *motor)
:RotaryEncoder(out, 20, motor)
{
}

/**
 * Runs whenever the slot is passed. If there is a motor, 
 * it will add or subract from the count based on the direction of the motor. 
 * If there is no motor, it will only add, and has no way to know the direction.
 */
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

/**
 * Start and attach the rotary encoder.
 * @return True if the out pin is a valid interrupt pin (2 or 3 on an Arduino Uno), otherwise false.
 */
boolean RotaryEncoder::begin()
{
  return attach();
}

/**
 * Resets the count of the encoder to zero.
 */
void RotaryEncoder::reset()
{
  noInterrupts();
  slotsPassed = 0;
  interrupts();
}

/**
 * Get the number of rotations the encoder has passed through.
 */
float RotaryEncoder::getRotations()
{
  int slotsPassedCopy;
  noInterrupts();
  slotsPassedCopy = slotsPassed;
  interrupts();
  return ((float)slotsPassedCopy) / slots;
}

/**
 * Get the number of degrees the encoder has passed through.
 */
float RotaryEncoder::getDegrees()
{
  return getRotations() * 360;
}

float RotaryEncoder::getRadians(){
  return getRotations() * 2 * PI;
}

/**
 * Get the distance traveled by the wheel the encoder is measuring.
 */
float RotaryEncoder::getDistanceTraveled(float wheelDiameter)
{
  return getRotations() * PI * wheelDiameter;
}

/**
 * Get the angle that the robot has turned in rotations. This ONLY applies to a "two wheel turn",
 * where one motor spins in one direction, and the other spins the opposite direction.
 */
float RotaryEncoder::getRobotTurnRotations(float wheelDiameter, float robotDiameter){
  return getDistanceTraveled(wheelDiameter) / (PI * robotDiameter);
}


/**
 * Get the angle that the robot has turned in degrees. This ONLY applies to a "two wheel turn",
 * where one motor spins in one direction, and the other spins the opposite direction.
 */
float RotaryEncoder::getRobotTurnDegrees(float wheelDiameter, float robotDiameter){
  return getRobotTurnRotations(wheelDiameter, robotDiameter) * 360;
}

/**
 * Get the angle that the robot has turned in radians. This ONLY applies to a "0ne wheel turn", where this motor turns the robot.
 * There is a dedicated method for two wheels in opposite directions. 
 */
float RotaryEncoder::getRobotTurnRadians(float wheelDiameter, float robotDiameter){
  return getRobotTurnRotations(wheelDiameter, robotDiameter) * 2 * PI;
}