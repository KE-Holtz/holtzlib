#include <Arduino.h>
#include <HoltzLib.h>

DCMotor rightDriveMotor(9,8,10);
DCMotor leftDriveMotor(7, 6, 5);

UltrasonicSensor sensor(12,11);

double Kp = 1;
double Ki = 0;
double Kd = 0;

double input;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  delay(500);
  input = sensor.getDistance(); 
  Serial.print("Sensor: ");
  Serial.println(sensor.getDistance());
  
  leftDriveMotor.drive(input / 4);
  rightDriveMotor.drive(input / 4);
}
