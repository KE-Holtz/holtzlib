#include <Arduino.h>
#include <HoltzLib.h>

//Creates a new motor with pins 9, 8, and 10 for in1, in2, and enA. Pin 10 is pwm.
DCMotor rightDriveMotor(9,8,10);
//Creates a new motor, running in reverse with pins 7, 6, and 5 for in3, in4, and enB. Pin 5 is pwm.
DCMotor leftDriveMotor(7, 6, 5, true);
//Creates a new ultrasonic sensor with pins 12 for trig and 11 for echo.
UltrasonicSensor sensor(12,11);


void setup()
{
  Serial.begin(9600);
}

double input;
double power = 0;
void loop()
{
  delay(500);
  
  //Power increases until it reaches 255, then goes back to 0
  power++;
  if(power > 255){
    power = 0;
  }
  
  input = sensor.getDistance(); 
  Serial.print("Distance (cm): ");
  Serial.println(input);

  //If input is greater than 100, run motors reversed. Otherwise, run them normally.
  //You could also put the boolean expression (input > 100) directly in drive() :
  //rightDriveMotor.drive(power, input > 100);
  if(input > 100) {
    rightDriveMotor.drive(power, true);
    leftDriveMotor.drive(power, true);
  } else {
    rightDriveMotor.drive(power);
    leftDriveMotor.drive(power);
  }
}
