/* UNO: sketch */
/*
	V FINAL 	no

	the initial SPI interface code for Arduino
	- mat

	TODO: adapt this into complete FC
*/
#include <SPI.h>

#define _BUFFER_SIZE 32

char buf[_BUFFER_SIZE];
volatile byte pos;
volatile boolean process_it;

void setup()
{
	Serial.begin(115200);   // debugging
        //SPI.setClockDivider(SPI_CLOCK_DIV8);
	Serial.println("Starting");

	// have to send on master in, *slave out*
	pinMode(MISO, OUTPUT);

	// MISO, slave mode
	SPCR |= _BV(SPE);

	// get ready for an interrupt 
	pos = 0;   // buffer empty
	process_it = false;

	// now turn on interrupts
        //SPI.setBitOrder( MSBFIRST );
	SPI.attachInterrupt();
}


/* Interrupt on comms
 * Write as little code as possible
 * If you change this the "Acceptable Baud Range" will need to be changed also.
 * SPDR must be assigned to a variable FIRST!!! and SPDR cannot be referrenced more than once!
 * Current baud = 100000Us
 */
ISR(SPI_STC_vect)
{
  byte c = SPDR;
        if( c == 0x0 ) process_it = true;
	if (pos < _BUFFER_SIZE)
                buf[pos++] = c;   
        else
                process_it = true;
}

// main loop - wait for flag set in interrupt routine
void loop(void)
{
	if (process_it)
	{
		buf[pos] = 0;

		Serial.print("\nMSG:  ");
		Serial.println(buf);
		pos = 0;
		process_it = false;
	}  

}  
