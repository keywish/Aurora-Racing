
#ifndef _INFRAREDTRACING_H_
#define _INFRAREDTRACING_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>


#define  IT_ALL_BLACK     0b00000
#define  IT_ALL_WHITE     0b11111
#define  IT_LEFT1         0b01111
#define  IT_LEFT2         0b00111
#define  IT_LEFT3         0b10111
#define  IT_CENTER        0b11011
#define  IT_RIGHT1        0b11101
#define  IT_RIGHT2        0b11100
#define  IT_RIGHT3        0B11110
// information for the interrupt handler
typedef enum {
	E_INFRARED_SENSOR_2,
    E_INFRARED_SENSOR_3,
	E_INFRARED_SENSOR_4,
	E_INFRARED_SENSOR_5
} E_INFRARWD_TRACING_MODE;


class InfraredTracing
{
private:
	byte InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3, InfraredTracingPin4, InfraredTracingPin5;
	byte value;
	double irDelayTime;

public:
	InfraredTracing(byte pin1, byte pin2);
	InfraredTracing(byte pin1, byte pin2, byte pin3);
	InfraredTracing(byte pin1, byte pin2, byte pin3, byte pin4);
	InfraredTracing(byte pin1, byte pin2, byte pin3, byte pin4, byte pin5);
	void begin();
	void end();
	boolean CheckBlack();
	E_INFRARWD_TRACING_MODE InfraredTracingMode;
	byte getValue(void);
};
#endif
