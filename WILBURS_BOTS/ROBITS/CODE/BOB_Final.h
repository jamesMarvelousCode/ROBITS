#include <Sparki.h> // include the sparki library
#include "pitches.h"
//C:\Program Files (x86)\SparkiDuino\hardware\arduino\avr\cores\arduino\Sparki.h

int RANUM = 0;
int COUNT = 0;
	
//----------------------------------------------------------------------------------------------------------
//	void setup() these instructions fire one time at startup. Any instructions that need to be executed before the program starts go here.
//---------------------------------------------------------------------------------------------------------	
void setup()
{
	happyBeep();
	sparki.gripperClose(7);
	delay(10000);
	sadBeep();
	sparki.gripperOpen(3);
	delay(3000);
	lookAbout();
	fastBusyBeep();
	sparki.servo(SERVO_CENTER);
	sparki.clearLCD();
	randomSeed(analogRead(0));
}

//----------------------------------------------------------------------------------------------------------
// void loop() this is the main method. The nature of robot programming, similar to an operating system, requires instructions to be executed continually in a loop. Calls to the main decision making methods go here.
//----------------------------------------------------------------------------------------------------------
void loop()
{
	RANUM = random(1, 100);

	sparki.RGB(RGB_GREEN);
    sparki.moveForward(); // start BOB moving forward

	pickRoute(); //teh majics
	
	behaviorsGO();
	
    delay(50);
}

//----------------------------------------------------------------------------------------------------------
// void pickDirection() this method fires if BOB finds himself within 20cm of an object; most likely he has found himself in a corner. Once this method fires, BOB will perform a more detailed sweep of his surroundings and make a decision bassed on what he sees.
//	RED/VIOLET
//----------------------------------------------------------------------------------------------------------
void pickDirection()
{   
	//pick variables
	int tempPing = 0, decision = 0, fourtyFiveDeg_Right = 0, ninetyDeg_Right = 0, fourtyFiveDeg_Left = 0, ninetyDeg_Left = 0;
	int FIFTEEN = 15;
	
	//centering servo before scan
	sparki.servo(SERVO_CENTER);

	//initial instructions before detailed scan. Remember, if these instructions execute, BOB has found himsef in a corner and needs to stop and look about to find his way
	sparki.moveStop();
	delay(50);    
      
	//get 45deg right ping
	sparki.servo(45);
	delay(100);
	fourtyFiveDeg_Right = sparki.ping();
	
	//get 90deg right ping
	sparki.servo(90);
	delay(100);
	ninetyDeg_Right = sparki.ping();

	//get 45deg left ping
	sparki.servo(-45);
	delay(200);
	fourtyFiveDeg_Left = sparki.ping();
      
	//get 90deg left ping
	sparki.servo(-90);//get 60deg left ping
	delay(100);
	ninetyDeg_Left = sparki.ping();
	
	//re-centering servo after scan
	sparki.servo(SERVO_CENTER);

	//make decisions for 5 different outcomes: 45degRight, 90degRight, 45degLeft, 90degleft, all <=15cm, and a default
	//1 = 45deg Right
	//2 = 90deg Right
	//3 = 45deg Left
	//4 = 90deg Left
	//5 = 180deg
	
	
	//make decision
	if (fourtyFiveDeg_Left >= fourtyFiveDeg_Right)
	{
		sparki.RGB(RGB_VIOLET);
		tempPing = fourtyFiveDeg_Left;
		fourtyFiveDeg_Left = tempPing;
		decision = 3;
	}
	else
	{
		sparki.RGB(RGB_VIOLET);
		tempPing = fourtyFiveDeg_Right;
		decision = 1;
	}
	if (ninetyDeg_Left > tempPing)
	{
		tempPing = ninetyDeg_Left;
		//sparki.print("90deg Left was > temp");//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		//sparki.print("Temp is now == ");//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		//sparki.println(tempPing);//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		//sparki.updateLCD();//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		decision = 4;
	}
	if (ninetyDeg_Right >= tempPing)
	{
		tempPing = ninetyDeg_Right;
		//sparki.print("90deg Right was > temp");//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		//sparki.print("Temp is now == ");//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		//sparki.println(tempPing);//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		//sparki.updateLCD();//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		decision = 2;
	}
	if (fourtyFiveDeg_Right <= FIFTEEN && fourtyFiveDeg_Left <= FIFTEEN && ninetyDeg_Right <= FIFTEEN && ninetyDeg_Left <= FIFTEEN)
	{
		//sparki.println("All pings <= 15!");//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		//sparki.updateLCD();//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
		decision = 5;
	}
	
	//sparki.print("Returning ");//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
	//sparki.print(decision);//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
	//sparki.println(" largeAdjustment()");//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
	//sparki.updateLCD();//REMOVE_PRINT_STATEMENTS_AFTER_TESTING
	largeAdjustment(decision);
}

//----------------------------------------------------------------------------------------------------------
//	int pickRoute() this method fires when BOB sees an object <=40cm ahead. When this method fires, BOB will glance right and left to decide which way he should go.
//	RED/VIOLET
//----------------------------------------------------------------------------------------------------------
void pickRoute()
{
	int cmAhead = 0, routeDecision = 0, leftPing = 0, rightPing = 0;
	
	//call edge detection
	edgeDetection();
	
    cmAhead = sparki.ping(); // BOB's primary ping
	
	if(cmAhead != -1) // make sure its not too close or too far
    { 
		sparki.RGB(RGB_RED);
		if (cmAhead <= 30)//far off obstacle detection
		{
			//begin looking for a better route
			//centering servo before scan
			sparki.servo(SERVO_CENTER);
			
			sparki.servo(30);
			delay(100);
			rightPing = sparki.ping();
			delay(100);
			
			sparki.servo(-30);
			delay(100);
			leftPing = sparki.ping();
			delay(100);
			
			//re-centering servo after scan
			sparki.servo(SERVO_CENTER);
			
			//decide which is the better option
			//0 default take no new action
			//1 swerve left
			//2 swerve right
			//3 call pickDirection()
			
			sparki.RGB(RGB_VIOLET);
			if (leftPing <= 30 && rightPing <= 30)
			{
				routeDecision = 3;
			}
			else if (leftPing < rightPing)
			{
				routeDecision = 2;
			}
			else if (leftPing >= rightPing)
			{
				routeDecision = 1;
			}
			else if (leftPing <= 40 && rightPing <= 40)
			{
				//recursive call here should act almost like a quick re-check; BOB's ping accuracy sucks...
				//At this point BOB has determined that there is an object directly ahead at <=40 cm ahead AND has glanced left and right and found that both routes have objects at <60 cm
				//SOOOOOOO at this point a quick re-check using super-fun recursion will help his navigation accuracy(I think)
				pickRoute();
			}
			else
			{
				routeDecision = 0;
			}
			smallAdjustment(routeDecision);
		}
    }
}

//----------------------------------------------------------------------------------------------------------
// void edgeDetection() this method is called at the beginning of pickDirection to find edges
// this method will ++Count to keep track of how many times bob detects the same edge. if count gets too high he turns around. also if RANUM is low enough it is used to give the turnAround variable a bit of variance
//----------------------------------------------------------------------------------------------------------
void edgeDetection()
{
	int edgeLeft   = sparki.edgeLeft();   // measure the left edge IR sensor
	int edgeRight  = sparki.edgeRight();  // measure the right edge IR sensor
	int turnAround = 100;
	int threshold = 200; // if below this value, no surface underneath

	sparki.RGB(RGB_WHITE);
	
	if (edgeLeft < threshold) // if no surface underneath left sensor
	{
		if (COUNT > 2)
		{
			if (RANUM > 6)
			{
				if (RANUM%2 == 0)
				{
					turnAround -= RANUM;
					sparki.moveRight(turnAround);
					COUNT = 0;
				}
				else
				{
					turnAround += RANUM;
					sparki.moveRight(turnAround);
					COUNT = 0;
				}
			}
			else
			{
				sparki.moveRight(turnAround);
				COUNT = 0;
			}
		}
		else
		{
			sparki.moveRight(30);
			COUNT++;
		}
	}

	if (edgeRight < threshold) // if no surface underneath right sensor
	{
		if (COUNT > 2)
		{
			if (RANUM > 6)
			{
				if (RANUM%2 == 0)
				{
					turnAround -= RANUM;
					sparki.moveLeft(turnAround);
					COUNT = 0;
				}
				else
				{
					turnAround += RANUM;
					sparki.moveLeft(turnAround);
					COUNT = 0;
				}
			}
			else
			{
				sparki.moveLeft(turnAround);
				COUNT = 0;
			}
		}
		else
		{
			sparki.moveLeft(30);
			COUNT++;
		}
	}
	
	sparki.moveForward();
}

//----------------------------------------------------------------------------------------------------------
// void largeAdjustment() call this method and send it the decision int generated by void pickDirection()
//	VIOLET
//----------------------------------------------------------------------------------------------------------
void largeAdjustment(int decision)
{
	int turnAround = 180;
	if (RANUM > 20)
	{
		if (RANUM%2 == 0)
		{
			turnAround -= RANUM;
		}
		else
		{
			turnAround += RANUM;
		}
	}
	
	sparki.RGB(RGB_VIOLET);
	switch(decision)
	{
		case 0:
			//pickRoute() gets called here because ALL previous methods failed to make a decision some how
			pickRoute();
			break;
		case 1:
			sparki.moveBackward(1); // back up 1 centimeter
			sparki.moveRight(45); // rotate right 30 degrees
			sparki.moveForward();
			break;
		case 2:
			sparki.moveBackward(1); // back up 1 centimeter
			sparki.moveRight(90); // rotate right 60 degrees
			sparki.moveForward();
			break;
		case 3:
			sparki.moveBackward(1); // back up 1 centimeter
			sparki.moveLeft(45); // rotate left 30 degrees
			sparki.moveForward();
			break;
		case 4:
			sparki.moveBackward(1); // back up 1 centimeter
			sparki.moveLeft(90); // rotate left 60 degrees
			sparki.moveForward();
			break;
		case 5:
			sparki.moveBackward(1); // back up 1 centimeter
			//determine +- 180 sum
			sparki.moveRight(turnAround); // rotate right 180 degrees +- about 10 degrees
			sparki.moveForward();
			break;
		default:
			//error action
			sadBeep();
			break;  
	}
}

//----------------------------------------------------------------------------------------------------------
//	void smallAdjustment() call this method and send it the decision int generated by void pickRoute()
//	RED
//----------------------------------------------------------------------------------------------------------
void smallAdjustment(int decision)
{
	sparki.RGB(RGB_VIOLET);
	switch(decision)
	{
		case 0:
			//pickRoute() gets called here because ALL previous methods failed to make a decision some how
			pickRoute();
			break;
		case 1:
			sparki.moveStop();
			sparki.moveLeft(25);
			delay(100);
			sparki.moveForward();
			break;
		case 2:
			sparki.moveStop();
			sparki.moveRight(25);
			delay(100);
			sparki.moveForward();
			break;
		case 3:
			pickDirection();
			break;
		default:
			//error action
			sadBeep();
			break;
	}
}

//----------------------------------------------------------------------------------------------------------
//	void behaviorsGO() this method checks the RANUM and fires a behavior if the params are right
//----------------------------------------------------------------------------------------------------------
void behaviorsGO()
{
	//switch for actual random behaviors
	switch(RANUM)
	{
		case 2:
			randomDirection();
			break;
		case 15:
			investigateObject();
			break;
		case 30:
			lookAbout();
			break;
		case 42:
			baladOfBobLegend();
			break;
		case 65:
			gripperFlex();
			break;
		case 99:
			spinningTheramin();
			break;
		default:
			//take no action
			break;
	}
}

//----------------------------------------------------------------------------------------------------------
//	void investigateObject() BEHAVIOR this method stops sparki and has him look around for a nearby object to check out
// 	BLUE
//----------------------------------------------------------------------------------------------------------
void investigateObject()
{  
	//pick variables
	int tempPing = 0, decision = 0, fourtyFiveDeg_Right = 0, ninetyDeg_Right = 0, fourtyFiveDeg_Left = 0, ninetyDeg_Left = 0, ping = 100;
	int THIRTY = 30;
	
	sparki.RGB(RGB_BLUE);
	//centering servo before scan
	sparki.servo(SERVO_CENTER);
	sparki.moveStop();
	busyBeep();
	delay(50);    
	  
	//get 45deg right ping
	sparki.servo(45);
	delay(100);
	fourtyFiveDeg_Right = sparki.ping();
	
	//get 90deg right ping
	sparki.servo(90);
	delay(100);
	ninetyDeg_Right = sparki.ping();

	//get 45deg left ping
	sparki.servo(-45);
	delay(200);
	fourtyFiveDeg_Left = sparki.ping();

	sparki.servo(-90);//get 60deg left ping
	delay(100);
	ninetyDeg_Left = sparki.ping();

	
	//re-centering servo after scan
	sparki.servo(SERVO_CENTER);

	//make decisions for 5 different outcomes: 45degRight, 90degRight, 45degLeft, 90degleft, all <=15cm, and a default
	//1 = 45deg Right
	//2 = 90deg Right
	//3 = 45deg Left
	//4 = 90deg Left
	//5 = 180deg
	
	//make decision
	if (fourtyFiveDeg_Left <= fourtyFiveDeg_Right)
	{
		tempPing = fourtyFiveDeg_Left;
		decision = 3;
	}
	else
	{
		tempPing = fourtyFiveDeg_Right;
		decision = 1;
	}
	if (ninetyDeg_Left < tempPing)
	{
		tempPing = ninetyDeg_Left;
		decision = 4;
	}
	if (ninetyDeg_Right <= tempPing)
	{
		tempPing = ninetyDeg_Right;
		decision = 2;
	}
	if (fourtyFiveDeg_Right >= THIRTY && fourtyFiveDeg_Left >= THIRTY && ninetyDeg_Right >= THIRTY && ninetyDeg_Left >= THIRTY)
	{
		decision = 5;
	}
	
	switch(decision)
	{
		case 0:
			//sad
			sadBeep();
			break;
		case 1:
			sparki.moveRight(45); // rotate right 30 degrees
			moveToInvestigate();
			break;
		case 2:
			sparki.moveRight(90); // rotate right 60 degrees
			moveToInvestigate();
			break;
		case 3:
			sparki.moveLeft(45); // rotate left 30 degrees
			moveToInvestigate();
			break;
		case 4:
			sparki.moveLeft(90); // rotate left 60 degrees
			moveToInvestigate();
			break;
		case 5:
			//sad
			sadBeep();
			break;
		default:
			//sad
			sadBeep();
			break;  
	}
}

//----------------------------------------------------------------------------------------------------------
//	void moveToInvestigate() BEHAVIOR SUBCLASS this method moves sparki close to an object to check out
//----------------------------------------------------------------------------------------------------------
void moveToInvestigate()
{
	int ping = 20;
	fastBusyBeep();
	while (ping > 10)
	{
		ping = sparki.ping();
		sparki.moveForward();
		delay(50);
	}
	
	sparki.moveStop();
	happyBeep();
	
	if(RANUM > 49)
	{
		sparki.moveRight(360);
		fastBusyBeep();
	}
	else
	{
		sparki.moveLeft(360);
		fastBusyBeep();
	}
	
	if(RANUM > 49)
	{
		sparki.moveRight(RANUM);
		fastBusyBeep();
	}
	else
	{
		sparki.moveLeft(RANUM);
		fastBusyBeep();
	}
	
}

//----------------------------------------------------------------------------------------------------------
//	void randomDirection() stops and picks a random direction
//----------------------------------------------------------------------------------------------------------
void randomDirection()
{
	//TODO fix
	busyBeep();
	if(RANUM > 49)
	{
		sparki.moveRight(RANUM);
		fastBusyBeep();
	}
	else
	{
		sparki.moveLeft(RANUM);
		fastBusyBeep();
	}
}

//----------------------------------------------------------------------------------------------------------
//	void lookAbout() BEHAVIOR sparki stops, looks around and chirps
//	RED
//----------------------------------------------------------------------------------------------------------
void lookAbout()
{
	sparki.moveStop();
	
	int loopInt = 0;
	busyBeep();
	sparki.RGB(RGB_RED);
	while (loopInt > -50)
	{
		sparki.servo(loopInt);
		delay(25);
		loopInt -= 2;
	}
	
	while (loopInt < 50)
	{
		sparki.servo(loopInt);
		delay(25);
		loopInt += 2;
	}
	
	while (loopInt > 0)
	{
		sparki.servo(loopInt);
		delay(25);
		loopInt -= 2;
	}
	
	happyBeep();
}

//----------------------------------------------------------------------------------------------------------
//	void baladOfBobLegend() BEHAVIOR stops to sing the baladOfBobLegend
//	INDIGO
//----------------------------------------------------------------------------------------------------------
void baladOfBobLegend()
{
	sparki.moveStop();
	sparki.RGB(RGB_INDIGO);
	//sing the balad of bob legend
	// notes in the melody:
	int melody[] = { NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_B3, NOTE_G3, NOTE_B3, NOTE_A3, NOTE_G3, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_C4,      
                  NOTE_E4, NOTE_D4, NOTE_C4, NOTE_B3, NOTE_B3, NOTE_C4, NOTE_B3, NOTE_G3, NOTE_B3, NOTE_A3, NOTE_G3, NOTE_A3, 0};

	// note durations: 4 = quarter note, 8 = eighth note, etc.:
	int noteDurations[] = { 4, 8, 8, 4, 4, 8, 8, 4, 4, 4, 4, 4, 8, 8, 8, 8, 4, 8, 8, 4, 4, 8, 8, 4, 4, 4, 4, 4, 2};
	
	for (int thisNote = 0; thisNote < 29; thisNote++) {
	
	if (thisNote == 0)
		sparki.moveLeft();
	
	if (thisNote == 4)
		sparki.servo(45);
	
	if (thisNote == 8)
		sparki.servo(-45);
	
	if (thisNote == 11)
		sparki.moveRight();
	
	if (thisNote == 12)
		sparki.servo(45);
	
	if (thisNote == 16)
		sparki.servo(-45);
	
	if (thisNote == 19)
		sparki.moveLeft();
	
	if (thisNote == 20)
		sparki.servo(45);
	
	if (thisNote == 24)
		sparki.servo(-45);
	
	if (thisNote == 27)
		sparki.moveRight();
	
	if (thisNote == 28)
		sparki.servo(0);
	
    int noteDuration = 1000/noteDurations[thisNote];
    sparki.beep(melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    sparki.noBeep();
  }
}

//----------------------------------------------------------------------------------------------------------
//	void gripperFlex() BEHAVIOR flexes his gripper while moving
//	ORANGE
//----------------------------------------------------------------------------------------------------------
void gripperFlex()
{
	int delayInt = 100;
	sparki.RGB(RGB_ORANGE);
	sparki.gripperClose(2);
	while(delayInt < 1500)
	{
		pickRoute();
		delayInt += 100;
	}
	
	delayInt = 100;
	sparki.gripperOpen(2);
	while(delayInt < 1500)
	{
		pickRoute();
		delayInt += 100;
	}
	
	delayInt = 100;
	sparki.gripperClose(2);
	while(delayInt < 2000)
	{
		pickRoute();
		delayInt += 100;
	}
	sparki.gripperOpen(1);
}

//----------------------------------------------------------------------------------------------------------
//	void happyBeep() sparkis happy beep
//	BLUE
//----------------------------------------------------------------------------------------------------------
void happyBeep()
{
	sparki.RGB(RGB_BLUE);
	sparki.beep(NOTE_D6, 125);
	delay(125);
	sparki.beep(NOTE_A7, 300);
	delay(300);
	sparki.beep(NOTE_D7, 125);
	delay(125);
	sparki.beep(NOTE_A7, 200);
	delay(200);
}

//----------------------------------------------------------------------------------------------------------
//	void sadBeep() sparkis sad beep
//	YELLOW
//----------------------------------------------------------------------------------------------------------
void sadBeep()
{
	sparki.RGB(RGB_YELLOW);
	sparki.beep(NOTE_A6, 400);
	delay(400);
	sparki.beep(NOTE_G4, 400);
	delay(400);
	sparki.beep(NOTE_D3, 400);
	delay(400);
}

//----------------------------------------------------------------------------------------------------------
//	void busyBeep() sparkis doing things beep
//----------------------------------------------------------------------------------------------------------
void busyBeep()
{
	sparki.RGB(RGB_YELLOW);
	sparki.beep(NOTE_C3, 200);
	delay(300);
	sparki.beep(NOTE_C3, 200);
	delay(300);
	sparki.beep(NOTE_C3, 200);
	delay(300);
	sparki.beep(NOTE_C3, 200);
	delay(300);
	sparki.beep(NOTE_G3, 200);
	delay(300);
	sparki.beep(NOTE_G3, 200);
	delay(300);
	sparki.beep(NOTE_G3, 200);
	delay(300);
	sparki.beep(NOTE_G3, 200);
	delay(300);
}

//----------------------------------------------------------------------------------------------------------
//	void fastBusyBeep() sparkis doing things beep
//----------------------------------------------------------------------------------------------------------
void fastBusyBeep()
{
	sparki.RGB(RGB_YELLOW);
	sparki.beep(NOTE_C3, 100);
	delay(150);
	sparki.beep(NOTE_C3, 100);
	delay(150);
	sparki.beep(NOTE_C3, 100);
	delay(150);
	sparki.beep(NOTE_C3, 100);
	delay(150);
	sparki.beep(NOTE_G3, 100);
	delay(150);
	sparki.beep(NOTE_G3, 100);
	delay(150);
	sparki.beep(NOTE_G3, 100);
	delay(150);
	sparki.beep(NOTE_G3, 100);
	delay(150);
}

//----------------------------------------------------------------------------------------------------------
//	void spinningTheramin() runs the spinning theramin for 360deg
//----------------------------------------------------------------------------------------------------------
void spinningTheramin()
{
	sparki.servo(SERVO_CENTER);
  
	for (int i = 0; i < 72; i++)
	{
		int note = sparki.ping();
		sparki.moveRight();
		delay(10);
	  
		if (note != -1)
		{
			sparki.beep(note*10);
		}

		if (note > 100)
		{
			sparki.noBeep();
		}

		delay(10); 
	}
}