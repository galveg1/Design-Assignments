 /*
 * task2.c
 *
 * Created: 3/14/2018 4:00:04 PM
 * Author : Guillermo Gálvez
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


int main ()
{
	DDRB |= (1 << DDB2); 	//PB2 set to ouput
	DDRD &= ~(1 << DDD2); 	//Set to input
	PORTD |= (1 << 2);		//Set Pull-up
	PORTB = 0;				//Initilize output to 0
	
		while (1)
		{
				if(!(PIND & (1 << PIND2)))	//Pull-up resistor switch to ground
				{							 
					_delay_ms(1000);		//Delay 1 second
					PORTB ^= (1 << PB2);	//LED_on
				}
				else
				{
					PORTB = 0;				//LED_off
				}
		}//end while(1)

}//Main



	