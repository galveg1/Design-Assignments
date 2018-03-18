/*
 * task1da2C.c
 *
 * Created: 3/11/2018 7:36:48 PM
 * Author : Guillermo Gálvez
 */ 

#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>


#include <avr/interrupt.h>



int main(void)
{
	DDRB = (1 << 2);	//PB2 is an output
	
    
   
    while (1) 
    {
   		 _delay_ms(250);		
   		 PORTB ^= (1 << 2);		//toggle PB2	     	
    }//end while (1)
}//end main


