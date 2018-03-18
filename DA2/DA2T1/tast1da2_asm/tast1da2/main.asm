;
; tast1da2.asm
;
; Created: 3/11/2018 11:26:25 AM
; Author : Guillermo Gálvez
start:
   
.equ counter = 128			;OCR value count for timer
clr r0   					;r0=0


ldi r1, (1<<2)				;for port configuration 
out DDRB, r1				;PB2 set as output
out PORTB, r0				;PORTB = 0
ldi r1, 0xff				;for port configuration
out DDRC, r1				;^^^^^^^^^^^^^^^^^^^^^^
out PORTC, r0				;PORTB = 0


ldi r1, (1<<0)				;lsb = 1
ldi r2, counter				;set loop control
clr r1						;counter set to 0

timeStrt:	out TCNT0, r0					;timer0 at 0
			out TCCR0A, ro					;normal mode
			ldi r3, (1<<CS00) | (1<<CS01)	;set 64 prescale
			out TCCR0B, r3


timeRpt:	in r4, TIFR0			;check overflow TOV0
			sbrs r4, 0				;skip next when TOV0 set
			rjmp timeRpt 			;repeat
		clr r3						;stop timer
		out TCCR0C, r3					
		ldi r3, (1<<TOV0)			;flag clr
		out TIFR0, r3					
		dec r2						;loop control -1
		brne timeStrt



                        
