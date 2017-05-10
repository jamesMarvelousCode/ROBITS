//Left Track - LTRACK - M1
//Right Track - RTRACK - M2
//Left Sonar - LSONAR
//LSONAR.vcc = black.5v
//LSONAR.trig = white.I/O_7
//LSONAR.echo = gray.I/O_6
//LSONAR.gnd = purple.gnd
//Right Sonar - RSONAR
//RSONAR.vcc = blue.vin
//RSONAR.trig = green.I/O_5
//RSONAR.echo = yellow.I/O_4
//RSONAR.gnd = orange.gnd

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
//Uncomment next line for servos
//#include <Servo.h> 

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);