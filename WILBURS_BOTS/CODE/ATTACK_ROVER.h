#include <Adafruit_SoftServo.h>
#include <NewPing.h>

//BE SURE TO CHECK THESE PARAMS AGAINST THE ROBOT'S SPECS BEFORE UPLOAD
#define SERVO1PIN 4
#define SERVO2PIN 3

#define TRIGGER_PIN 0
#define ECHO_PIN 1
#define MAX_DISTANCE 200

Adafruit_SoftServo servo_left, servo_right;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

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

    if (sonar.ping_cm() < OBSTACLE && sonar.ping_cm() != 0)
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
        if (sonar.ping_cm() < ATTACK_DISTANCE && sonar.ping_cm() != 0)
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

