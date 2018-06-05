#include "aurora.h"
RGBLed rgbled_A3(7,A3);
aurora::aurora(unsigned int m1,unsigned int m2,unsigned int m3,unsigned int m4,unsigned int led,unsigned int battery,unsigned int servo,unsigned int servo_degree)
{
	this->Bin1_pin=m1;
	this->Bin2_pin=m2;
	this->STBY_pin=m3;
	this->PWMB_pin=m4;
	this->led_pin=led;
	this->battery_adc=battery;
  this->servo_pin = servo;
  this->default_degree = servo_degree;
}

void aurora::init(void)
{
	bot_type = "hummer-bot";
	addr = 0x01;
	hbot_speed = 0 ;
  
	pinMode(Bin2_pin, OUTPUT);
	digitalWrite(Bin1_pin, LOW); // When not sending PWM, we want it low
 
	pinMode(PWMB_pin, OUTPUT);
	digitalWrite(PWMB_pin, LOW); // When not sending PWM, we want it low
  
	pinMode(Bin1_pin, OUTPUT);
	digitalWrite(Bin1_pin, LOW); // When not sending PWM, we want it low
 
	pinMode(STBY_pin, OUTPUT);
	digitalWrite(STBY_pin, LOW); // When not sending PWM, we want it low
  
	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, LOW);
	pinMode(battery_adc, INPUT);

   pinMode(servo_pin, OUTPUT);
  ForwardServo.attach(servo_pin);
  ForwardServo.write(default_degree);
}

void aurora::setSpeed(int s)
{
	hbot_speed = s;
}

int aurora::getBattery(void)
{

}

/*
int aurora::SendInfo(void)
{
	//

}
*/
void aurora::goBack(void)
{
    //Serial.println("goBack");
    int value = (hbot_speed/10)*25;	 //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
    analogWrite(Bin2_pin,255);
    analogWrite(Bin1_pin,0);
    analogWrite(PWMB_pin,value);
    analogWrite(STBY_pin,255);
     rgbled_A3.setColor(0,value-50,value-50,value-50);
    rgbled_A3.show(); 
}

void aurora::goForward(void)
{
    //Serial.println("goForward");
    int value = (hbot_speed/10)*25;   //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
    Serial.println("aserv ");
    analogWrite(Bin1_pin,255);
    analogWrite(Bin2_pin,0);
    analogWrite(STBY_pin,255);
    analogWrite(PWMB_pin,value);
    rgbled_A3.setColor(0,0,value-50,0);
    rgbled_A3.show(); 
}

void aurora::turnLeft()
{
  int value = (hbot_speed / 10) * 25; //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  //Serial.println("turnLeft");
  analogWrite(Bin1_pin, 255);
  analogWrite(Bin2_pin, 0);
  analogWrite(STBY_pin, 255);
  analogWrite(PWMB_pin, value);
  rgbled_A3.setColor(2,25,15,0);
  rgbled_A3.setColor(1,0,0,0);
  rgbled_A3.show(); 

}

void aurora::turnRight()
{
  int value = (hbot_speed / 10) * 25; //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
 // Serial.println("turnRight");
  analogWrite(Bin1_pin, 255);
  analogWrite(Bin2_pin, 0);
  analogWrite(STBY_pin, 255);
  analogWrite(PWMB_pin, value);
  rgbled_A3.setColor(1,25,15,0);
  rgbled_A3.setColor(2,0,0,0);
  rgbled_A3.show(); 
}

void aurora::keepStop()
{
  int value = (hbot_speed / 10) * 25; //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
    Serial.println("keepStop");
  digitalWrite(Bin1_pin,LOW );
  digitalWrite(Bin2_pin, HIGH);
  analogWrite(STBY_pin,255);
  digitalWrite(PWMB_pin,LOW);
  rgbled_A3.setColor(0,value-50,0,0);
  rgbled_A3.show();   
}



void aurora::drive(int digree)
{
//Serial.print("set degree :");
//Serial.print(digree);
//Serial.print("\n");
    int value = (hbot_speed/10)*25;	 //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
    float f  ;
     if (digree == 0xFFFF)
  {
  ForwardServo.write(default_degree);
    keepStop();
  }

  if ( digree >= 60 && digree <= 120 )
  {
    goForward();
    ForwardServo.write(default_degree);
  }

  if (digree >= 331)
  {
    turnRight();
    ForwardServo.write(120);
  }
  if (digree <= 20)
  {
   ForwardServo.write(120);
    turnRight();
  }
  if ( digree >= 241 && digree <= 290)
  {
    goBack();
    ForwardServo.write(default_degree);
  }
  if ( digree >= 160 && digree <= 210 )
  {
    turnLeft();
  ForwardServo.write(50);
  }
   if (digree >=361 ||digree <= 0 )
   {
      keepStop();
     ForwardServo.write(90);
  }
  
   if (digree > 21 && digree < 50) 
    {
    ForwardServo.write(110);
    turnRight();
    }
    if (digree > 121 && digree < 150)
    {
    turnLeft();
    ForwardServo.write(60);
    }
    if (digree > 211 && digree < 240) 
    {
        goBack();
      ForwardServo.write(70);
       
    } 
 if (digree > 291 && digree < 330  )
{
      goBack();
      ForwardServo.write(115);
 }
   
}
