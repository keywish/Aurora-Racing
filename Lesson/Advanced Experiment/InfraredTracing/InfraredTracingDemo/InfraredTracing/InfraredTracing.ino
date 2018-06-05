#include "InfraredTracing.h"
#include "Servo.h"

#define MOTOR_L2   4   //Bin1
#define MOTOR_L1   2  //Bin2
#define PWMB   6  //PWMB
#define STBY   7  //STBY
#define SERVO_PIN  5
Servo ForwardServo;
int Speed = 200;
int BaseDegree = 90;
InfraredTracing mIT(A0, A1, A2, A4, A5);

void setup()
{
    Serial.begin(9600);
    pinMode(MOTOR_L2, OUTPUT);
    pinMode(MOTOR_L1, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(STBY, OUTPUT);
    ForwardServo.attach(SERVO_PIN);
    ForwardServo.write(BaseDegree);
    mIT.begin();
}

void loop()
{
    switch(mIT.getValue()) {
      
    case IT_ALL_BLACK:
    case IT_ALL_WHITE:
        Serial.println("NO BLACK LINE");
        Speed = 0;
        ForwardServo.write(90);
        break;
    case IT_CENTER:
        ForwardServo.write(BaseDegree);
        Serial.println("go forward");
        Speed = 100;
        break;
    case IT_RIGHT1:
        Serial.println("turn right1");
        ForwardServo.write(BaseDegree + 35);
        Speed = 70;
        break;
    case IT_RIGHT2:
        Serial.println("turn right2");
        ForwardServo.write(BaseDegree + 45);
        Speed = 70;
        break;
    case IT_LEFT1:
        ForwardServo.write(BaseDegree - 45);
        Serial.println("turn left1");
        Speed = 70;
        break;
    case IT_LEFT2:
        Serial.println("turn left2");
        ForwardServo.write(BaseDegree - 30);
        Speed = 70;
        break;
      }
    digitalWrite(MOTOR_L2, HIGH);
    digitalWrite(MOTOR_L1, LOW); //the speed value of motorA is val
    analogWrite(PWMB, Speed);
    digitalWrite(STBY, HIGH);
}
