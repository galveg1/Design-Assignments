/*
 * da4T3galveg1.c
 *
 * Created: 4/14/2018 03:23:32 PM
 * Author : Guillermo Galvez
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//Variable Declarations
volatile unsigned int PWMOUT; // output for PWM duty cycle

void timekeeper();
void adc_init();

int main(void)
{
	//Port initialization
	DDRB = 0x02;		//Set PB1 OC1A as output to generate PWM
	
	timekeeper();
	adc_init();
		
	while (1)
	{
		ADCSRA |= (1 << ADSC);				//Start conversion
		while((ADCSRA & (1 << ADIF)) == 0);		//Wait for conversion to finish
		PWMOUT = (ADC & 0x03FF) * 24 / 10+800;	//OCR1A value for duty cycle
		OCR1A = PWMOUT;
		
		
	}//end while(1)
	
}//end MAIN

void timekeeper()
{
	//TIMER1 initialization - generate 50Hz PWM
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);			// Fast PWM, Non-inverted mode
	TCCR1B |= (1 << WGM13) | (1<< WGM12) | (1 << CS11);	// 8 prescaling
	//TIMSK1	|= (1 << TOIE1);
	ICR1 = 9999;										// top value for timer1
}

void adc_init()
{
	DDRC	= 0;			//Set PORTC as input for adc
	DIDR0	= 0x01;			//Disable Digi input on ADC0
	
	ADMUX = 0;				//
	ADMUX |= (1 << REFS0);	//Use Vcc Ref voltage
	//ADMUX |= (1 << ADLAR);	//Left adjust ADC Reg, 8-bit Resolution
	
	//Enable ADC, Start Conv, Set Auto Trig Enable
	ADCSRA |= (1 << ADEN);	//Enable ADC
	//ADCSRA |= (1 << ADATE); //Set ADC Auto Trig
	//ADCSRA |= (1 << ADIE);	//Enable Interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //8MHz, Pre-Scale 128 = 62500KHz
	ADCSRA |= (1 << ADSC);	//Start Conversion
	
	ADCSRB = 0;			//Free running mode
	sei();				//Enable interrupts
}