#ifndef _INFRAREDTRACING_H_
#define _INFRAREDTRACING_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

#ifdef ME_PORT_DEFINED
#endif // ME_PORT_DEFINED
#ifndef __AVR_ATmega32U4__

#define USECPERTICK 100  // microseconds per clock interrupt tick

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define _GAP 5000 // Minimum map between transmissions

#ifdef F_CPU
#define SYSCLOCK F_CPU     // main Arduino clock
#else
#define SYSCLOCK 16000000  // main Arduino clock
#endif


#define _GAP 5000 // Minimum map between transmissions
#define GAP_TICKS (_GAP/USECPERTICK)


#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_ENABLE_PWM     (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM    (TCCR2A &= ~(_BV(COM2B1)))
#define TIMER_ENABLE_INTR    (TIMSK2 = _BV(OCIE2A))
#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_INTR_NAME      TIMER2_COMPA_vect
#define TIMER_CONFIG_KHZ(val) ({ \
  const uint8_t pwmval = F_CPU / 2000 / (val); \
  TCCR2A = _BV(WGM20); \
  TCCR2B = _BV(WGM22) | _BV(CS20); \
  OCR2A = pwmval; \
  OCR2B = pwmval / 3; \
})

#define TIMER_COUNT_TOP      (SYSCLOCK * USECPERTICK / 1000000)
#if (TIMER_COUNT_TOP < 256)
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR2A = _BV(WGM21); \
  TCCR2B = _BV(CS20); \
  OCR2A = TIMER_COUNT_TOP; \
  TCNT2 = 0; \
})
#else
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR2A = _BV(WGM21); \
  TCCR2B = _BV(CS21); \
  OCR2A = TIMER_COUNT_TOP / 8; \
  TCNT2 = 0; \
})
#endif

#endif // !__AVR_ATmega32U4__

#define  IT_ALL_BLACK     0b00000
#define  IT_ALL_WHITE     0b11111
#define  IT_LEFT1         0b01111
#define  IT_LEFT2         0b10111
#define  IT_CENTER        0b11011
#define  IT_RIGHT1        0b11101
#define  IT_RIGHT2        0b11110

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

