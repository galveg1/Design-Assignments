
 /*
DA2 task3
 * Created: 3/11/2018 7:36:48 PM
 * Author : Guillermo Gálvez
*/ 
#define F_CPU 8000000
#include<avr/io.h>
#include<util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>


/*
Task 3 
repeat task 1 T = .5s generate a 50% DC
8,000,000/64 = 125,000=f, T=1/125,000 8µS

1/125,000 = 8µS * 62,500 cycles =.5sec

62,500/2 = 31,250 * .8µS=250ms or .25s

T_on  = .25s
T_off = .25s

T = .5s

*/

int main(void)
{
	DDRB |= (1 << 2); 									//Set PB2 output	
	PORTB = 0;											//initialize to 0

	TCCR0A = 0;											//normal mod
	TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);	//prescaler 64
	TCNT0  = 0;											//initialize counter
	
	unsigned int i;	//loop control variable


	while(1)
	{		
		for(i=0; i < 62500; i++)
		{
			if(i == 31250)
				PORTB ^= (1 << 2);
			if(i == 62499)	
				PORTB ^= (0 << 2);
				


		}

		
	}//end while(1)

}//end main

