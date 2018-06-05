#ifndef aurora_H
#define aurora_H
#include "Arduino.h"
#include "Servo.h"
#include "RGBLed.h"
class aurora {

private :
    String bot_type;
    unsigned char addr;
    int degree;
public :
    aurora(unsigned int m1,unsigned int m2,unsigned int m3,unsigned int m4,unsigned int led,unsigned int battery,unsigned int servo, unsigned int servo_degree);
    unsigned char led_pin;
    unsigned char battery_adc;
    unsigned char temp;
    unsigned char hbot_speed ;
    unsigned char Bin1_pin;
    unsigned char Bin2_pin;
    unsigned char STBY_pin;
    unsigned char PWMB_pin;
    unsigned char servo_pin;
    unsigned int  default_degree;

    Servo ForwardServo; 
    void init(void);
    void goForward(void);
    void goBack(void);
    void turnLeft(void);
    void turnRight(void);
    void keepStop(void);
    void drive(int degree);
    void setSpeed(int s);
    int getBattery(void);
};
#endif
