#include "Servo.h"// Because the steering wheel of the car is driven by a rudder, the program must contain the corresponding library file.
int E1 = 4; //Bin1
int M1 = 2; //Bin2****** The car only uses a DC motor drive, so the TB6612 drive chip can only be used all the way。
int E2 = 6; // Defines the PWMB interface as 6.
int M2 = 7; // The STBY interface is defined as 7.
int servo_pin = 5;// The steering gear is defined as No 5, and the rudder wire is inserted directly into the expansion board to the corresponding position.
Servo ForwardServo;// Create a control object for the steering wheel
void setup()
{
	Serial.begin(9600);
	pinMode(M1, OUTPUT);
	pinMode(M2, OUTPUT);
	pinMode(E1, OUTPUT); // Initialization of motor-driven IO for output mode
	ForwardServo.attach(servo_pin);
	ForwardServo.write(90);// The initial steering gear is 90°, and the center position.
}
void loop()
{
	int left1,centre,right1,left2,right2;// Defines the 5-way sensor variable name
	left1=analogRead(A0);
	left2=analogRead(A1);
	centre=analogRead(A2);
	right1=analogRead(A4);
	right2=analogRead(A5);//read five sensor
    if ((right1 >= 975)&&(right2 >= 975)&&(centre <= 8)&&(left2 >= 975)&&(left1 >= 975)) {
	   //*******forward*******//
		ForwardServo.write(90);// The steering gear stays 90° when driving forward.
		int val=80;// PWM value, 2--255, the greater the value, the faster the car in the track, so we recommend that the speed is not too fast
		digitalWrite(E1,HIGH);
		digitalWrite(M1, LOW); 
		analogWrite(E2,val);
		digitalWrite(M2,HIGH);// High level normal work
	} else if ((right1 <= 8)&&(centre >= 975)&&(left1 >= 975)&&(left2 >= 975)&&(right2 >= 975)) {
		//***turn right1***//
		ForwardServo.write(125);// Rotate right from 90°to 120°and rotate the wheel 30 ° to the right.
		int val=70;
		digitalWrite(E1,HIGH);
		digitalWrite(M1, LOW); 
		analogWrite(E2,val);
		digitalWrite(M2,HIGH); // High level normal work
   } else if((right1 >= 975)&&(right2 >= 975)&&(centre >= 975)&&(left1 <= 8)&&(left2 >= 975)) {
	   //***Turn left1***//
		ForwardServo.write(45);// Rotate left from 90°to 40°and rotate the wheel 50 ° to the left.。
		int val=70;
		digitalWrite(E1,HIGH);
		digitalWrite(M1, LOW); 
		analogWrite(E2,val);
		digitalWrite(M2,HIGH); // High level normal work
    } else if((right2 >= 975)&&(centre >= 975)&&(left2 <= 8)&&(right1= 975)&&(left1 >= 975)) {
		//***Turn left2***//
		ForwardServo.write(50);// Rotate left from 90°to 50°and rotate the wheel 40 ° to the left.。
		int val=70;
		digitalWrite(E1,HIGH);
		digitalWrite(M1, LOW); 
    } else  if((right2 <= 8)&&(centre >= 975)&&(left2 >= 975)&&(left1 >= 975)&&(right1 >= 975)) {
     //***turn right2***//
		ForwardServo.write(120);// Rotate right from 90°to 105°and rotate the wheel 15 ° to the right.
		int val=70;
		digitalWrite(E1,HIGH);
		digitalWrite(M1, LOW); 
		analogWrite(E2,val);
		digitalWrite(M2,HIGH); // High level normal work
   } else if((centre <= 8)&&(left2 <= 8)&&(right2 <= 8)&&(left1 <= 8)&&(right1 <= 8)) {
		   //*******Stop*******//
		ForwardServo.write(90);// When stopped, let the steering gear return to its original position for easy execution of the next action.   
		int val=0;
		digitalWrite(E1,HIGH);
		digitalWrite(M1, LOW); 
		analogWrite(E2,val);
		digitalWrite(M2,LOW); // The low level is on standby, at which point the car stops.
   }
}
