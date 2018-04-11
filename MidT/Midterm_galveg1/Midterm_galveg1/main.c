/*
 * Midterm_galveg1.c
 *
 * Created: 4/7/2018 12:23:56 PM
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
/*~3.5min timer1 Counter 65536*25 * T = ~3.5 minutes*/
volatile uint8_t fifteenPlus = 25;	//


/*Function Declarations*/
void USART_Init();
void ADC_Init();
void outputChr(unsigned char c);	
void outputStr(unsigned char *c);
void readTemp();

void AT_Tx(char *t);
//void append(unsigned char *one, unsigned char *two);

/*Interrupt Service Routines*/			
ISR(ADC_vect)
{
	ADCSRA	|= (1 << ADIF);	//Reset flag
	adcValue = ADCH;		//MSB 8-bits of ADC form left shift of ADLAR
}//end ISR(ADC_vect(


ISR(TIMER1_OVF_vect)
{
	TIFR1 |= (1 << TOV1);	//Clr Flag
	fifteenPlus++;
	
	char seeTemp[8];
	float	lm34_0;		//For ASCII Temp output
	
	ADCSRA |= (1 << ADSC);						//Start Conversion
	while((ADCSRA & (1 << ADIF)) == 0);			//Wait for conversion to finish
	
	/*Conversion to °F*/
	lm34_0 = (adcValue * 5.0 / 0x100) * 100.0;	//(ADC * 5 = 200 /256) * 100
	
	dtostrf(lm34_0, 5, 2, seeTemp);				//Float to char conversion


		/*Build Strings for AT+ commands*/
		unsigned char CIPStart[]	= "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
		unsigned char CIPSend[]		= "AT+CIPSEND=51\r\n";
		unsigned char Data[]		= "GET /update?api_key=X5GNOJ6AFIEO0XRP&field1=";
		unsigned char temp0			= "t";
		unsigned char temp1			= "\r\n";
		unsigned char CIPClose[]	= "AT+CIPCLOSE\r\n";
			
	if(fifteenPlus>=25)		//Timer
	{
	
		
		
		_delay_ms(2000);
		outputStr(CIPStart);	//Send Start String
		
		_delay_ms(2000);
		outputStr(CIPSend);		//Number of Char being sent
		
		_delay_ms(2000);
		outputStr(Data);		//Get command sent
		outputStr(seeTemp);			//Temperature string added to end of Get command
		outputStr("\n\r");		//Enter key
		
		fifteenPlus = 0;		//Reset timer
		
	}//end if
	
}//end ISR(TIMER1_OVF_vect)

/**************************************************
***************************************************
***************************************************
MAIN***********************************************
*/
int main(void)
{
	TCCR1B	|= (1 << CS12) | (1 << CS10);	//Set prescale 1024
	TIMSK1  |= (1 << TOIE1);					//enable OVF interrupt
	
	unsigned int oneFive = 0;
		
	ADC_Init();			//initialize ADC
	USART_Init();		//UART initialization
	
	/*WiFi Loggin Credentials*/
	unsigned char CWJAPLogIn[]	= "AT+CWJAP=\"-7-\",\"985677221174\"";
	
	_delay_ms(2000);
	outputStr(CWJAPLogIn);
	outputStr("\r\n");	
						
    while (1) 
    {
	
    }//end while(1)
	
}//end main

/*USART initialization function*/
void USART_Init()
{
		
	/*Set Baud Rate*/
	UBRR0H = (MYUBRR>>8);	//Shift MSB "top" of UBRR0H	0100 0100 >> 8 -> UBRR0H 0000 0000
	UBRR0L = MYUBRR;		//UBRR0L 0100 0100
			
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);	 //Enable Rec and Trans
	UCSR0B |= (1 << RXCIE0);				 //Enable Rec INT
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //Set frame 8-bit, 1 STP
}//end USART_int 

/*ADC initialization function*/
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

/*Character print function*/
void outputChr(unsigned char c)
{
	UDR0 = c;	//Display Char on Serial 
	_delay_ms(800);
}//end outputChr

/*String print function*/
void outputStr(unsigned char *c)
{
	unsigned int i = 0;	//loop control
	while(c[i] != 0)
	outputChr(c[i++]);
}//end outpuStr

