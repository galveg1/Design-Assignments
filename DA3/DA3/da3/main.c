/*
 * da3.c
 *
 * Created: 3/25/2018 9:45:22 AM
 * Author : Guillermo Gálvez
 */ 
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#define F_CPU 8000000UL

#define FOSC 16000000			//Frequency
#define BAUD 9600				//Baud Rate
#define MYUBRR FOSC/16/BAUD-1	//Automatic BAUD rate calculation

volatile uint8_t adcValue;

/*Function Declarations*/
void USART_Init();
void ADC_Init();
void outputChr(unsigned char c);	
void outputStr(char *c);
void outputNum(float t);
void readTemp();

/*Interrupt Service Routines*/			
ISR(ADC_vect)
{
	ADCSRA	|= (1 << ADIF);	//Reset flag
	adcValue = ADCH;		//MSB 8-bits of ADC form left shift of ADLAR
}//end ISR(ADC_vect(

ISR(TIMER1_OVF_vect)
{
	TIFR1 |= (1 << TOV1);	//Clr Flag
	readTemp();	
}//end ISR(TIMER0_OVF

//Attempt at using stream file Didn't work
// static int outChr(char t, FILE *stream);	//Output function
// static FILE usartout = FDEV_SETUP_STREAM(outChr, NULL, _FDEV_SETUP_WRITE);	//Stream pointer

int main(void)
{
	//stdout = &usartout;	//Attempt at output stream 
	
	ADC_Init();			//initialize ADC
	USART_Init();		//UART initialization
	
	TCCR1B = (1 << CS12);	//Set prescale 256
	OCR1A = 31094;			//TCNT1 for 1s real time 31250 reduced to account for
							//additional delays in print functions
	TIMSK1 |= (1 << TOIE1); //enable OVF interrupt
	
			
    while (1) 
    {
					
    }//end while(1)
	
}//end main

void USART_Init()
{
	/*Attempt at stream didn't work
	unsigned int BR;
	BR = ((F_CPU/16)/BAUD)-1;
	UBRR0H = (unsigned char) (BR>>8);
	UBRR0L = (unsigned char) BR;
	*/
	
	/*Set Baud Rate*/
	UBRR0H = (MYUBRR>>8);	//Shift MSB "top" of UBRR0H	
	UBRR0L = MYUBRR;		//Shift LSB "bot" of UBRR0L
			
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);	 //Enable Rec and Trans
	UCSR0B |= (1 << RXCIE0);				 //Enable Rec INT
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //Set frame 8-bit, 1 STP
}//end USART_int 

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

void outputChr(unsigned char c)
{
	UDR0 = c;	//Display Char on Serial 
	_delay_ms(800);
}//end outputChr

void outputNum(float t)
{
	UDR0 = t;
	_delay_ms(500);
}

void outputStr(char *c)
{
	unsigned int i = 0;	//loop control
	while(c[i] != 0)
	outputChr(c[i++]);
}//end outpuStr

void readTemp()
{
	char seeTemp[8];
	float	lm34_0;		//For ASCII Temp output
	float	lm34_1;		//For showing valued of ADC
	
	ADCSRA |= (1 << ADSC);							//Start conversion
		while((ADCSRA & (1 << ADIF)) == 0);			//Wait for conversion to finish
		
		
		
		/*To show ADC value as float on graph at ~78°F it's ~40*/
		/*On terminal it will show up as some ASCII character depending on value*/
		lm34_1 = adcValue;
		outputNum(lm34_1);	
								
				
		/*Conversion to °F*/			
		lm34_0 = (adcValue * 5.0 / 0x100) * 100.0;	//(ADC * 5 = 200 /256) * 100
									
		dtostrf(lm34_0, 5, 2, seeTemp);				//Float to char conversion
		
		outputStr(" Temp is: ");	//Count
		outputStr(seeTemp);
		
		outputStr("\n\r");
					
		//Prints non-sense
		/*
		printf("Temp is: ");
		
		outputStr(seeTemp);
				
		printf("°F");
				
		printf('\n');
		printf('\r');
		*/
}

//Attempt at Stream File output Prints non-sense ************************************
/*
void outputStr(char *c)
{
	while((*c != '\0'))
	{
		while(!(UCSR0A & (1 << UDRE0)));	//Wait until Tx buffer is ready
		UDR0 = *c;	//Send char to Tx
		c++;		//Next char
	}
}//end outpuStr

static int outChr(char t, FILE *stream)
{
	while(!(UCSR0A & (1 << UDRE0)));	//Wait for UDR to clr
	UDR0 = t;	//Send Char to stream Tx
	return 0;
}//End OutChr

*/

