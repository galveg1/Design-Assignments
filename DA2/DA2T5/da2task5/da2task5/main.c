 /*
 * task5.c
 *
 * Created: 3/14/2018 4:00:04 PM
 * Author : Guillermo Gálvez
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



ISR(INT0_vect)
{
	EIFR |= (1 << INTF0);
	
	if(!(PIND & (1 << PIND2)))	//Pull-up resistor switch to ground
	{							 
		_delay_ms(1000);		//Delay 1 second
		PORTB ^= (1 << PB2);	//LED_on
	}
	else
	{
		PORTB = 0;				//LED_off
	}
	
	
}//end ISR(INT0_vect)



int main ()
{
	DDRB |= (1 << DDB2); 	//PB2 set to ouput for LED
	DDRD &= ~(1 << PD2); 	//Set to input for switch
	PORTD |= (1 << PD2);	//Set Pull-up

	EIMSK |= (1 << INT0);	//activate extern int
	EICRA |= (0 << ISC01) | (1 << ISC00);	//activiate external int on rise
	PORTB = 0;								//Initilize output to 0
	sei();	//enagle global int

	
		while (1)
		{
			
		}//end while(1)
}//end Main