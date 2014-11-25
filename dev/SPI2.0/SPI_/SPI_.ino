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

volatile Motor motor_A;
volatile Motor motor_B;
volatile Motor motor_C;
volatile Motor motor_D;
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
	motor_A.pin = mpA;
	motor_B.pin = mpB;
	motor_C.pin = mpC;
	motor_D.pin = mpD;
 
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
		motor_A.newSpeed.data = *b;
		b++;
		motor_B.newSpeed.data = *b;
		b++;
		motor_C.newSpeed.data = *b;
		b++;
		motor_D.newSpeed.data = *b;
		pos  = 0;
		break;
	}
	if( motor_A.newSpeed.data != motor_A.oldSpeed.data )
		setSpeed(motor_A);
	if( motor_B.newSpeed.data != motor_B.oldSpeed.data )
		setSpeed(motor_B);
	if( motor_C.newSpeed.data != motor_C.oldSpeed.data )
		setSpeed(motor_C);
	if( motor_D.newSpeed.data != motor_D.oldSpeed.data )
		setSpeed(motor_D);
}

void setSpeed(Motor m) {
	/* only output different speed, do before call*/
	analogWrite( m.pin, m.newSpeed.int_t ); 
	m.oldSpeed = m.newSpeed;
	#ifdef DEBUG
		Serial.print(m.pin);
		Serial.print(pinToID(m.pin));
		Serial.print(m.newSpeed.int_t);
		Serial.print('\n');
	#endif
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
	digitalWrite(LED_PIN, BLINK?HIGH:LOW );
	if( !BLINK ) {
		motorArm();
	}
	BLINK = !BLINK;
	if( pos != 0 )
		COMS = true;
	else
		delay(100);
}

