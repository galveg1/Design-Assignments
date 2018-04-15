/*
 * da4T1.c
 *
 * Created: 4/13/2018 08:35:25 PM
 * Author : Guillermo Galvez
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>


//Global variables
volatile uint8_t speed;
int toggle = 0;


//Functions Declarations
void ADC_Init();
void timeKepper();
void interrupts();
void on_off();

ISR(ADC_vect)
{
	ADCSRA	|= (1 << ADIF); //set ADIF 1
}//end ADC_vect


ISR(TIMER0_OVF_vect)
{
	//PORTB = 0x01;	//Toggle on Start of new period 
	
	ADCSRA |= (1 << ADSC);	//Start Conversion
	while((ADCSRA & (1 << ADIF)) == 0);
	
	speed = ADCH;
	
	//Control DC from 0 to 95 of total TIMER1 Ticks at 1024 Pre-Scale
	//OC1A Drives motor and Toggles on compare of of OCR1A	
	//Creating a PWM wave
	if(speed >= 242) //242 95% of ADCH
	{
		OCR1A = 7422;	//95% of 7813 total timer ticks for 1024 Pre-Scale Max
	}
	else
	{
		OCR1A = speed * 32.3;	//Conversion of 8-bit ADCH to Timer1 counter range
	}	
}//end TIMER0_OVF_vect

//This was a failure unable to keep
//Motor only stays off while pressed
ISR(INT0_vect)
{
	EIFR |= (1 << INTF0);
	_delay_ms(1000);				//Debouncing
	while(!(PIND & (1 << PIND2)) == 0);	//Pull-up resistor switch to ground

}//end ISR(INT0_vect)


int main(void)
{
	DDRB = 0x02;			//Set PINB1 as output
	DDRD	|= (1 << DDD6);	//Set to output for OCA0
	;
	OCR1A = 25000;			//Arbritary initial value for OCR1A
	
	//Functions to initialize 
	//ADC, TIMER1 and Interrupts	
	ADC_Init();
	timeKepper();
	interrupts();
		
	while (1)
	{		
	}//end while(1)
	
}//end MAIN

void ADC_Init()
{
	DDRC	= 0;			//Set PORTC as input for adc
	DIDR0	= 0x01;			//Disable Digi input on ADC0
	
	ADMUX = 0;				//Sets Mux selection bits to 0 ADC0 used
	ADMUX |= (1 << REFS0);	//Use Vcc Ref voltage selectin 01
	ADMUX |= (1 << ADLAR);	//Left adjust ADC Reg, ADCH 8-bit Resolution
	
	//Enable ADC, Start Conv, Set Auto Trig Enable
	ADCSRA |= (1 << ADEN);	//Enable ADC
	ADCSRA |= (1 << ADATE); //Set ADC Auto Trig
	ADCSRA |= (1 << ADIE);	//Enable Interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); //8MHz, Pre-Scale 64 = 125KHz
	ADCSRA |= (1 << ADSC);	//Start Conversion
	
	ADCSRB = 0;			//Free running mode
	sei();				//Enable interrupts
	
}//end ADC_Init

void timeKepper()
{
	TCCR1A	|= (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);	//Clear OC1A on Compare Match, set	OC1A at BOTTOM (non-inverting mode)
	TCCR1B	|= (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10); //PWM Fast mode 1024 Pre-Scale
	TIMSK1	|= (1 << TOIE1);
}//end timekeeper

void interrupts()
{
	DDRD	|= (1 << PD2); 	//Set to input for switch
	PORTD	|= (1 << PD2);	//Set Pull-up

	EIMSK |= (1 << INT0);	//Enable extern INT0
	EICRA |= (1 << ISC01);	//Enanble external INT0 on rise
}//enterrupt

