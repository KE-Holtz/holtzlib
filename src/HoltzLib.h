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
  boolean currentDirectionReversed;

public:
  DCMotor(uint8_t in1, uint8_t in2, uint8_t powerPin, boolean reversed);
  DCMotor(uint8_t in1, uint8_t in2, uint8_t powerPin);

  void drive(int power, boolean reverse);
  void drive(int power);

  void stop();

  boolean isReversed();
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



class Interrupter
{
private:
  static void interruptRoutine0() {instances[0]->onInterrupt();}
  static void interruptRoutine1() {instances[1]->onInterrupt();}
  static Interrupter* instances[2];
public:
  Interrupter(uint8_t pin);
protected:
  boolean attach(int mode = RISING);
  void detatch();

  virtual void onInterrupt() = 0;

  uint8_t pin;
  uint8_t interruptNum;
};

class RotaryEncoder: public Interrupter
{
private:
  int slots;
  DCMotor *motor;
  volatile int slotsPassed;
  void countSlot();

public:
  RotaryEncoder(uint8_t out, int slots = 20, DCMotor *motor = nullptr);

  boolean begin();
  void reset();

  float getDegrees();
  float getRotations();
  float getDistance(float wheelDiameter);

protected:
  void onInterrupt() override;
};
#endif
