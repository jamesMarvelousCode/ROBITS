//This sketch is for small trinket powered rover ROBITS using the parallax PING))) sensor

#include <Adafruit_SoftServo.h>

#define SERVO1PIN 0
#define SERVO2PIN 1

Adafruit_SoftServo servo_left, servo_right;

//connect the sonar signal pin to this pin on the trinket
const int SONAR = 2;

//constants for drive direction NO FORGET TO TUNE SERVOS
const int LEFT_FORWARD = 180;
const int RIGHT_FORWARD = 10;
const int LEFT_BACKWARD = 0;
const int RIGHT_BACKWARD = 180;
const int SERVO_STOP = 90;

//DO NO MESS WITH THESE!
const int DRIVE_DELAY = 1;
const int SONAR_DELAY = 15;

//distance in cm that philip diverge
const int OBSTACLE = 10;
const int ATTACK_DISTANCE = 7;

//ping durations
const int HIGH_PING = 5;
const int LOW_PING = 2;

const int LONG_PAUSE = 2000;
const int SHORT_PAUSE = 50;
const int SHORT_TURN = 200;
const int SHORT_DELAY = 15;
const int TURN_WIGGLE = 50;

const int STAB_COUNT = 7;
const int POST_ATTACK_REVERSE = 100;
const int ATTACK_MODE_COOLDOWN = 600000;

//vars for ping return duration and distance in cm
long duration, cm;

bool attack_mode_on;

int ranum, attack_mode_timer;

//one-time-at-startup-set-up
//attaching/centering servos etc.
void setup()
{
    //inititalize variables
    ranum = 0;
    attack_mode_timer = 0;
    attack_mode_on = false;

	//attach the servos
	servo_left.attach(SERVO1PIN);
	servo_right.attach(SERVO2PIN);
}

//robots primary loop; this repeats until you turn off the robot
void loop()
{
    if (attack_mode_timer >= ATTACK_MODE_COOLDOWN)
    {
        attack_mode_on = true;
        attack_mode_timer = 0;
    }

    if (PING() < OBSTACLE)
    {
        DIVERGE();
    }
    else if (attack_mode_on)
    {
        ATTACK_MODE();
    }
    else
    {
        FORWARD();
    }
    attack_mode_timer++;
}

void DIVERGE()
{
    STOP(SHORT_PAUSE);
    BACKWARD(SHORT_PAUSE);
    TURN();
}

long PING()
{
    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(SONAR, OUTPUT);
    digitalWrite(SONAR, LOW);
    delayMicroseconds(LOW_PING);
    digitalWrite(SONAR, HIGH);
    delayMicroseconds(HIGH_PING);
    digitalWrite(SONAR, LOW);

    // The same pin is used to read the signal from the PING))): a HIGH
    // pulse whose duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(SONAR, INPUT);
    duration = pulseIn(SONAR, HIGH);

    // convert the time to cm and return
    return microseconds_to_centimeters(duration);  
}

void FORWARD()
{
	servo_left.write(LEFT_FORWARD);
    servo_left.refresh();
    servo_right.write(RIGHT_FORWARD);
    servo_right.refresh();
    delay(DRIVE_DELAY);
}

void BACKWARD(int time)
{
    while (time > 0)
    {
        servo_left.write(LEFT_BACKWARD);
        servo_left.refresh();
        servo_right.write(RIGHT_BACKWARD);
        servo_right.refresh();
        delay(DRIVE_DELAY);
        time--;
    }
}

void TURN()//add a variance input param
{
    ranum = random(1, 100);

    if (ranum > 50)
    {
        RIGHT_TURN(SHORT_TURN);
    }
    else 
    {
        LEFT_TURN(SHORT_TURN);
    }
}

void LEFT_TURN(int time)
{
    while (time > 0)
    {
        servo_left.write(LEFT_BACKWARD);
        servo_left.refresh();
        servo_right.write(SERVO_STOP);
        servo_right.refresh();
        delay(DRIVE_DELAY);
        time--;
    }
    STOP(SHORT_PAUSE);
}

void RIGHT_TURN(int time)
{
    while (time > 0)
    {
        servo_left.write(SERVO_STOP);
        servo_left.refresh();
        servo_right.write(RIGHT_BACKWARD);
        servo_right.refresh();
        delay(DRIVE_DELAY);
        time--;
    }
    STOP(SHORT_PAUSE);
}

void STOP(int time)
{
    while (time > 0)
    {
        servo_left.write(SERVO_STOP);
        servo_left.refresh();
        servo_right.write(SERVO_STOP);
        servo_right.refresh();
        delay(DRIVE_DELAY);
        time--;
    }
}

void ATTACK_MODE()
{
    STOP(SHORT_PAUSE);
    LEFT_TURN(TURN_WIGGLE);
    RIGHT_TURN(TURN_WIGGLE);
    for (int i; i < STAB_COUNT;)
    {
        if (PING() < ATTACK_DISTANCE)
        {
            BACKWARD(SHORT_DELAY);
            i++;
        }
        else
        {
            FORWARD();    
        }
    }
    STOP(SHORT_PAUSE);
    attack_mode_on = false;
    BACKWARD(POST_ATTACK_REVERSE);
    TURN();
}

//the speed of sound is 340m/s or 29 microseconds per cm
//the sensor returns the pings round trip time in microseconds
//so we divide that by 29 to get the round trip distance 
//and by half again to get distance from the object
long microseconds_to_centimeters(long microseconds)
{
	return microseconds / 29 / 2;
}
