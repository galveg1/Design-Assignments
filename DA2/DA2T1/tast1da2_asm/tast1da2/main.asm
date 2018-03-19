;
; task1da2.asm
;
; Created: 3/11/2018 11:26:25 AM
; Author : Guillermo Gálvez
start:
   
;TCNT = (8MHz/1024 * .5/2)-1 = 1952
;TCNT = 0x07A0
;**********************************
.org 0

LDI R16, (1 << 2)	;To set PB2
OUT DDRB, R16		;PB2 as output
LDI R17,0			;needed to toogle led
OUT PORTB,R17
LDI R20,5			;to set prescaler
STS TCCR1B,R20		;Prescaler: 1024
begin:
RCALL delay			;calling timer to wait for 1 sec
EOR R17,R16			;XOR to toogle led
OUT PORTB,R17
RJMP begin			;repeating i.e, while(1)
delay:
 LDS R29, TCNT1H	;loading upper bit of counter to R29
 LDS R28, TCNT1L	;loading lower bit of counter to R28
 CPI R28,0xA0		;comparing if lower is 0xA0
 BRSH body
 RJMP delay
body:
 CPI R29,0x07		;comparing if higher is 0x07
 BRSH done
 RJMP delay
done:
 LDI R20,0x00
 STS TCNT1H,R20		;resetting the counter to 0 for next round
 LDI R20,0x00
STS TCNT1L,R20		;resetting the counter to 0 for next round
RET



                        
