/*
 * tas4da2C.c
 *
 * Created: 3/11/2018 7:36:48 PM
 * Author : Guillermo Gálvez
 */
#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

#define l_on	PORTB |=	(1 << PORTB2)
#define l_off	PORTB &=	~(1 << PORTB2)
#define l_tog	PINB  |=	(1 << PINB2)

ISR(TIMER0_OVF_vect)
{
	l_tog;
	_delay_ms(250);	//LED_on half the period .5s
	l_tog;
	_delay_ms(250);
}


int main(void)
{
	DDRB |= (1 << DDB2);	//set ouput
		

	TCCR0B |= (1 << CS02) | (1 << CS00);	//Pre-Scaler 1024
	TIMSK0 |= (1 << TOIE0);					//Set-OVF 
	sei();



    while (1) 
    {
    	
    }//end while (1)
}//end main

