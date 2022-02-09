#define F_CPU 4000000UL        /* using default clock 4MHz*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <math.h>
#include "util/delay.h"
#include "UART_1_AVR128DA64.h"
#include <stdbool.h>

void ms_timer(void);
unsigned long int millis(void);
bool state = false; //bool condition state consider as false
unsigned long int millis_sec = 0; //0 to 4,294,967,295 long value 

void ms_timer(void)
{
	TCA1.SINGLE.CNT = 0;//
	TCA1.SINGLE.PER = 2000; //1MS TIME-PERIOD
	TCA1.SINGLE.CTRLA |=(1<<0); //timer enable
	TCA1.SINGLE.INTCTRL |=(1<<0);// overflow interrupt enable
       //Writing the OVF bit to ‘1’ enables the overflow/underflow interrupt.
}

ISR(TCA1_OVF_vect) 
{
	millis_sec++; //milli_sec increment
	TCA1.SINGLE.INTFLAGS |= (1<<0); //overflow/Underflow Interrupt Flag
	//This flag is set either on a TOP (overflow) or BOTTOM (underflow) condition, depending on the WGMODE setting.
	//The OVF flag is not cleared automatically. It will be cleared only by writing a ‘1’ to its bit location.
}

unsigned long int millis(void)
{
	return millis_sec;  //return the millis sec
}

int main()
{   
	USART1_init(9600);
	sei();
	PORTB.DIR |= (1<<2); //PORTB PIN2 DIRECTION PIN SET
	//PORTB.OUTSET |=(1<<2);
	ms_timer(); //CALL ms_timer
	unsigned long int test; 
	while (1)
	{
		_delay_ms(500);
		test = millis_sec; 
		USART1_sendInt(test);
		USART1_sendString("hello");
	}
}
