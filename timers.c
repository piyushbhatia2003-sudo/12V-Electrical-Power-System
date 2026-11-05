//timer.c: Timer initialization

#include "eps.h"

//timer initialize function

volatile uint32_t timer_ms = 0;

void Timer_Initialize() {

	
	//Timer 0 Compare A Match Interrupt
	TCCR0A = (1 << WGM01); //CTC Mode (Clear Timer on Compare Match)
	TCCR0B = (1 << CS01) | (1 << CS00); //prescaler is 64 
	OCR0A = 249; //value must be under 250 for 1ms interrupt 
	//F_CPU / 64 = 16 MHz / 64 = 250000 Hz or 250 ticks/s (0-249)
	TIMSK0 = (1 << OCIE0A); //Timer0 Compare Match A interrupt is enabled 

}

ISR(TIMER0_COMPA_vect) { //Timer interrupt service routine
	timer_ms++; //timer counter goes up by 1ms
}