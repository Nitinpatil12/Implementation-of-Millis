/*
 * temp.c
 *
 * Created: 09-02-2022 10:02:45
 *  Author: AGVA
 */ 
#define F_CPU 4000000UL        /* using default clock 4MHz*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <math.h>
#include "util/delay.h"

#define RTC_EXAMPLE_PERIOD 5119

#include "UART_1_AVR128DA64.h"

void RTC_init(void); //FUNCTION PROTOTYPE DECLARATION

void RTC_init(void) //FUNCTION DEFINATION
{
	uint8_t temp;
	temp =CLKCTRL.XOSC32KCTRLA; //clear enable bit
	temp &= ~CLKCTRL_ENABLE_bm;
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
	while(CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm)
	{
		;
	}
	//SEL=0 use external crystal

	temp =CLKCTRL.XOSC32KCTRLA;
	temp &=~CLKCTRL_SEL_bm;
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);

	//Enable oscillator
	temp =CLKCTRL.XOSC32KCTRLA;
	temp |=CLKCTRL_ENABLE_bm;
	ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);

	while(RTC.STATUS >0)
	{
		;
	} //RTC period is set
	//set period
	RTC.PER= RTC_EXAMPLE_PERIOD ;
	
	
	RTC.CLKSEL=RTC_CLKSEL_OSC32K_gc;
	
	RTC.DBGCTRL |=RTC_DBGRUN_bm;

	RTC.CTRLA =RTC_PRESCALER_DIV32_gc | RTC_RTCEN_bm | RTC_RUNSTDBY_bm;
	RTC.INTCTRL |=RTC_OVF_bm;

}
inline void print(void)
{
	USART1_sendString("Hello World!\r\n");
}
ISR(RTC_CNT_vect)
{
	RTC.INTFLAGS = RTC_OVF_bm;  //set OVF Bit
	print();
}
int main()
{
	USART1_init(9600);
	sei();
	RTC_init();
	while (1)
	{
	}
}