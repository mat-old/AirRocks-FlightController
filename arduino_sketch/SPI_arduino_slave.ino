// SPI INTERFACE
// Adapted from Nick Gammon's https://gist.github.com/chrismeyersfsu/3317769#file-arduino_spi_slave-pde
// February 2011

// MOTOR CONTROL
// Basel Al-Rudainy https://github.com/baselsw/BlueCopter/tree/master/BlueCopter
// 6 april 2013


#define DEBUG
#define LED_PIN  13
#define BUF_SIZE 32
#define MSG_SIZE 4
#define BUF_END  28
#include <SPI.h>
#include "SPI_support.h"

void setSpeed ( Motor m );
void setup    ( void );
void loop     ( void );
void motorArm ( void );
void motorInit( void );

Motor motor_A;
Motor motor_B;
Motor motor_C;
Motor motor_D;
volatile byte pos = 0;

byte buf[BUF_SIZE] = {0,};
const byte * bufEnd;
char * bufStr;
bool  COMS  = false
	, BLINK = false;

void setup(void) {
	/* data setup */
	bufStr = (char *)buf;
	bufEnd = buf + BUF_END;
	motor_A.usepin(mpA);
	motor_B.usepin(mpB);
	motor_C.usepin(mpC);
	motor_D.usepin(mpD);
 
	#ifdef DEBUG
		Serial.begin(115200);
		delay(500);
		Serial.println("setup... in 3 seconds");
		delay(3000);
	#endif

	/* SPI slave setup */
	pinMode(MISO, OUTPUT);
	SPCR |= _BV(SPE);
	SPI.attachInterrupt();

	/* initial init & arm */
	motorInit();
	motorArm();

	/* BLINK for 'waiting on coms' state */
	pinMode     (LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);
}
 
 
/* define interrupt */
ISR(SPI_STC_vect)
{
	/*  SPDR is volatile, store in local */
	byte b = SPDR;
	if (pos < BUF_END)
		buf[pos++] = b;
	else
		pos = 0;
}

void loop(void)
{
	if (!COMS) waitingBlink();
	
	delay(100);
	byte * b = buf;
	while( b != bufEnd ) {
		if( *(b++) != 0xA ) continue;
		/* save a register and increment pointer */
		motor_A.newSpeed = *b;
		b++;
		motor_B.newSpeed = *b;
		b++;
		motor_C.newSpeed = *b;
		b++;
		motor_D.newSpeed = *b;
		pos  = 0;
		COMS = true;
		break;
	}
	motor_A.update_if_change();
	motor_B.update_if_change();
	motor_C.update_if_change();
	motor_D.update_if_change();
}

void motorInit(void) {
	pinMode( mpA, OUTPUT );
	pinMode( mpB, OUTPUT );
	pinMode( mpC, OUTPUT );
	pinMode( mpD, OUTPUT );
}

void motorArm(void) {
	analogWrite( mpA, MOTOR_ZERO_LEVEL );
	analogWrite( mpB, MOTOR_ZERO_LEVEL ); 
	analogWrite( mpC, MOTOR_ZERO_LEVEL );
	analogWrite( mpD, MOTOR_ZERO_LEVEL );
}


void waitingBlink(void) {
	digitalWrite(LED_PIN, HIGH );
	if( !BLINK )
		motorArm();
	else
		Serial.println("\rwaiting on coms...");
	BLINK = !BLINK;
	if( pos != 0 )
		COMS = true;
	else
		delay(100);

	digitalWrite(LED_PIN, LOW );
	delay(200);
}

