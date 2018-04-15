/*
 * da4_galveg1.c
 *
 * Created: 4/11/2018 05:01:07 PM
 * Author : Nova Supreme
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

volatile uint8_t speed; //variable to hold ADC value
//uint8_t speed_Low	= 0;
//uint16_t ten_bit	= 0;

ISR(ADC_vect)
{
	ADCSRA		|= (1 << ADIF); //set ADIF 1
	speed		= ADCH;
	
	//OCR1AL		= ADCL;
	//OCR1AH		= ADCH;
	
	

}//end ADC_vect

ISR(TIMER1_OVF_vect)
{	

}//end OVF_vect

ISR(TIMER1_COMPA_vect)
{
	_delay_ms(2);
}//end COMP1_vect

void my_delay();

int main(void)
{
	DDRD	= 0b00001111; //set PIND0-PIND3 to output
	TIFR1	|= (1 << OCF1A);	//Enable Out Comp A match INT 
	TIMSK1	|= (1 << OCIE1A);		//enable CompA INT
	TCCR1A	 = 0;
	TCCR1B	 = 0;
	

	//set to CTC mode
	TCCR1A	 = 0; //set WGM11 = 0, WGM10 = 0
	TCCR1B	|= (1 << WGM12) | (1 << CS12); //set WGM13 = 0, WGM12 = 1 CTC, Pre-scale = 256
	//set Output Compare Register to calculated count(compared
	OCR1A = 156;
	
	
	DDRD = 0xFF; //set D port to output
	
	ADC_Init();		//initialize ADC
	
	sei();			//Enable Interrupts
	
	while (1)
	{
		ADCSRA |= (1 << ADSC);	//Start Conversion
		while((ADCSRA & (1 << ADIF)) == 0);
		OCR1A		= speed * .75;
		
		PORTD = 1; //black
		my_delay();
		PORTD = 2; //green
		my_delay();
		PORTD = 4; //red
		my_delay();
		PORTD = 8; //blue
		my_delay();
		
		/*
		PORTD = 1; //black
		while((TIFR1 & (1 << OCF1A)) == 0);
		PORTD = 4; //green
		while((TIFR1 & (1 << OCF1A)) == 0);
		PORTD = 2; //red
		while((TIFR1 & (1 << OCF1A)) == 0);
		PORTD = 8; //blue
		while((TIFR1 & (1 << OCF1A)) == 0);
		*/
		/*
		PORTD = 1; //black
		_delay_ms(2);
		PORTD = 2; //red
		_delay_ms(2);
		PORTD = 4; //red
		_delay_ms(2);
		PORTD = 8; //blue
		_delay_ms(2);
		*/
		
		
		
	}//end while
}//end main


void ADC_Init()
{
	DDRC	= 0;			//Set PORTC as input for adc
	DIDR0	= 0x01;			//Disable Digi input on ADC0
	
	ADMUX = 0;				//
	ADMUX |= (1 << REFS0);	//Use Vcc Ref voltage
	ADMUX |= (1 << ADLAR);	//Left adjust ADC Reg, 8-bit Resolution
	
	//Enable ADC, Start Conv, Set Auto Trig Enable
	ADCSRA |= (1 << ADEN);	//Enable ADC
	ADCSRA |= (1 << ADATE); //Set ADC Auto Trig
	ADCSRA |= (1 << ADIE);	//Enable Interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); //8MHz, Pre-Scale 64 = 125KHz
	ADCSRA |= (1 << ADSC);	//Start Conversion
	
	ADCSRB = 0;			//Free running mode
	sei();				//Enable interrupts
	
}//end ADC_Init

void my_delay()
{
	while(TCNT1 != OCR1A);
	TCNT1 = 0;
	//while((TIFR1 & (1 << OCF1A)) == 0);
}