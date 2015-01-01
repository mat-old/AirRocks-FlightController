
#include <SPI.h>

//#define DEBUG
#define BUF_SIZE 64
#define MSG_SIZE 4
#define BUF_END  60
#define TRANSMISSION_DELAY 50000

#define mpA 6
#define mpB 10
#define mpC 9
#define mpD 5
#define MOTOR_ZERO_LEVEL  125
#define MOTOR_ARM_START  140
#define MOTOR_MAX_LEVEL  254
/*  speedX...  */
int spA, spB, spC, spD;

volatile byte pos = 0;

byte buf[BUF_SIZE] = {0,};
const byte * bufEnd;
char * bufStr;

void setup(void) {
	/* data setup */
	bufStr = (char *)buf;
	bufEnd = buf + BUF_END;
 
	#ifdef DEBUG
		Serial.begin(115200);
		delay(500);
		Serial.println("setup... in 3 seconds");
	#endif

	delay(3000);
	/* SPI slave setup */
	pinMode(MISO, OUTPUT);
	SPCR |= _BV(SPE);
	SPI.attachInterrupt();

	/* initial init & arm */
	motorInit();
	delay(3000);
	motorArm();

}
 
 
ISR(SPI_STC_vect) {
	/*  SPDR is volatile, store in local */
	byte b = SPDR;
	if (pos < BUF_END)
		buf[pos++] = b;
	else
		pos = 0;
}

void loop(void) {
	bool change = false;
	byte * b = buf;
	if( pos > 0 ) {
		while( b != bufEnd ) {
			/* find start of data */
			if( *(b++) != 0xA ) continue;

			spA = *b;
			b++;
			spB = *b;
			b++;
			spC = *b;
			b++;
			spD = *b;

			pos  = 0;
			change = true;
			break;
		}
	}

	#ifdef DEBUG
		Serial.print("A=");
		Serial.print(spA);
		Serial.print(" B=");
		Serial.print(spB);
		Serial.print(" C=");
		Serial.print(spC);
		Serial.print(" D=");
		Serial.print(spD);
		Serial.print("\n");
	#endif
	if(change) {
		analogWrite( mpA, (int)spA );
		analogWrite( mpB, (int)spB );
		analogWrite( mpC, (int)spC );
		analogWrite( mpD, (int)spD );
	} else
	delayMicroseconds(TRANSMISSION_DELAY);
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
