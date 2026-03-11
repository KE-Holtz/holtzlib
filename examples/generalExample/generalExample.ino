#include <Arduino.h>
#include <HoltzLib.h>


/**
 * Replace anything in <angle brackets> with your own code!
 * 
 * Step 1: Create motors and sensors
 * To create a motor, use this syntax:
 * DCMotor <variable name>(<in1 pin>, <in2 pin>, <enA pin>);
 * The variable name can be whatever you want, for example, "rightWheel"
 * in1, in2, and ENA are the pin numbers on the arduino that connect to the corresponding pin on the motor controller.
 * Make sure that the wire that goes to ENA is connected to a PWM pin on the arduino. If it's connected to a pin with ~, you're all set!
 * 
 * If you want a motor to run in reverse, you can use this syntax:
 * DCMotor <variable name>(<in1 pin>, <in2 pin>, <enA pin>, true);
 * This is helpful if you have two motors that face opposite directions on the robot.
 * 
 * To create the ultrasonic sensor, use this syntax:
 * UltrasonicSensor <variable name>(<trig pin>, <echo pin>);
 * Just like the motor, replace trig pin with the pin on the arduino that connects to trig on the ultrasonic sensor, same with echo.
 * 
 * To create the rotary encoder, use this syntax:
 * RotaryEncoder <variable name>(<pin>);
 * The RotaryEncoders use a special kind of pin called an interrupt pin. The arduino UNO only has two of these,
 * so you MUST connect the encoder to pin 2 or 3.
 * 
 * The way these encoders work, they will not know the difference between fowards and backwards. 
 * If you want, you can use this slightly more complex syntax
 * RotaryEncoder <variable name(<pin>, &<motor>);
 * Replace motor with the name of your motor variable.
 * Now, the encoder will ask the motor which way it is driving, and will use that to figure out if it is going fowards or backwards
 */

//Creates a new motor with pins 9, 8, and 10 for in1, in2, and enA. Pin 10 is pwm.
DCMotor rightDriveMotor(9,8,10);

//Creates a new motor, running in reverse with pins 7, 6, and 5 for in3, in4, and enB. Pin 5 is pwm.
DCMotor leftDriveMotor(7, 6, 5, true);

//Creates a new ultrasonic sensor with pins 12 for trig and 11 for echo.
UltrasonicSensor sensor(12,11);

//Creates a new encoder on pin 3, which will be put on the right drive motor.
//It will count up when the motor is spinning fowards, and down when spinning backwards
RotaryEncoder encoder(3, &rightDriveMotor);


void setup()
{
  /**
   * Step 2: Setup
   * There are a few things that need to happen in setup.
   */

  /**
   * Run Serial.begin(9600) if you want to print to the console in your code.
   * This is helpful for testing, but won't be helpful on the real robot.
   */
  Serial.begin(9600);

  /**
   * Start your encoders using .begin()
   * If you forget this, they will NOT work.
   * This also returns a boolean, true if the pin is valid, and false otherwise. 
  */
  if (encoder.begin())
  {
    Serial.println("pin is OK!");
  } else
  {
    Serial.println("pin is NOT OK! Use pin 2 or pin 3");
  }

  //If you are using a servo, you should attatch it here as well
}

void loop()
{
 //Step 3: Running motors and getting output from encoders and sensors
 
 /**
  * To drive the motor, use the drive method:
  * 
  * <motor name>.drive(<power>);
  * Power can be any number from 0 to 255, where 255 is max speed.
  * 
  * If you want to control the direction of the motor, use this syntax instead:
  * <motor name>.drive(<power>, <is reversed?>);
  * Power is the same as before. <is reversed> can be true to drive backwards, and false to drive fowards 
  */

  //Run both motors ahead, max speed
  rightDriveMotor.drive(255);
  leftDriveMotor.drive(255);

/**
 * There are multiple options for getting measurements from encoders. You can:
 *  1) Get the number of rotations the encoder has taken
 *  2) Get the number of degrees the encoder has traveled through
 *  3) Get the distance that a wheel attached to the encoder has traveled
 */
  int encoderRotations = encoder.getRotations();
  int encoderDegrees = encoder.getDegrees();
  /**
   * To get the distance, you need to input the diameter of the wheel.
   * The units will match the units you put in, so if you enter a diameter in cm,
   * the distance will be in cm.
  */
  int distanceTraveled = encoder.getDistance(5.0);

  /** 
   * To get the distance measured by an ultrasonic sensor, simply use the get distance method.
   * It returns the distance in cm
  */

  int ultrasonicDistance = sensor.getDistance();

  Serial.print("Encoder rotations: ");
  Serial.println(encoderRotations);

  Serial.print("Encoder degrees: ");
  Serial.println(encoderRotations);

  Serial.print("Encoder distance: ");
  Serial.println(encoderRotations);

  Serial.print("Ultrasonic distance: ");
  Serial.println(ultrasonicDistance);

  delay(100);
}
