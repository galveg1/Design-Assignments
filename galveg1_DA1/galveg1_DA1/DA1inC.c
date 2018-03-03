/*
 * DA1inC.c
 *
 * Created: 3/3/2018 8:41:50 AM
 * Author : F-YH5B
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

int main()
{

 while(1)
 {
	short num=0x0222;
	unsigned char msb=num[15:8];
	unsigned char lsb=num[7:0];



	for(num=0; num < 299 ;num++)
	{
		eeprom_write(num, *num[])

	}

 }//end while(1)



}