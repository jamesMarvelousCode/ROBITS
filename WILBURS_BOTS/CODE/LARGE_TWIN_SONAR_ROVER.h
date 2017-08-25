#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <NewPing.h>

//create the motorshield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

//assign motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

//define sonar params
#define LEFT_TRIGGER_PIN 7
#define LEFT_ECHO_PIN 6
#define RIGHT_TRIGGER_PIN 5
#define RIGHT_ECHO_PIN 4
#define MAX_DISTANCE 100

//create sonar objects
NewPing leftSonar(LEFT_TRIGGER_PIN, LEFT_ECHO_PIN, MAX_DISTANCE);
NewPing rightSonar(RIGHT_TRIGGER_PIN, RIGHT_ECHO_PIN, MAX_DISTANCE);

void setup()
{
	// put your setup code here, to run once:
	AFMS.begin();
	leftMotor->setSpeed(100);
	rightMotor->setSpeed(100);
}

void loop()
{
	// put your main code here, to run repeatedly:
	if (ping() > 15 && ping() !=0)
	{
		leftMotor->run(BACKWARD);
		rightMotor->run(FORWARD);
		delay(10);
	}
	else
	{
		leftMotor->run(FORWARD);
		rightMotor->run(FORWARD);
		delay(10);
	}
}

int ping()
{
	return leftSonar.ping_cm();
}


//https://learn.adafruit.com/curiebot-arduino-101-mini-robot-rover/make-your-robot-autonomous
