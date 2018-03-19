;
; task3da2_asm.asm
;
; Created: 3/18/2018 8:44:07 PM
; Author : Guillermo Gálvez
;


.org 0

LDI R16,32
SBI DDRB, 2 ;PC5 as output
LDI R17,0 ;needed to toogle led
OUT PORTC,R17
LDI R20,5 ;to set prescaler
STS TCCR1B,R20 ;Prescaler: 1024
begin:
RCALL delay ;calling timer to wait for 1 sec
EOR R17,R16 ;XOR to toogle led
OUT PORTC,R17
RJMP begin ;repeating i.e, while(1)
delay:
 LDS R29, TCNT1H ;loading upper bit of counter to R29
 LDS R28, TCNT1L ;loading lower bit of counter to R28
 CPI R28,0x83 ;comparing if lower is 0x83
 BRSH body
 RJMP delay
body:
 CPI R29,0x1e ;comparing if higher is 0x1e
 BRSH done
 RJMP delay
done:
 LDI R20,0x00
 STS TCNT1H,R20 ;resetting the counter to 0 for next round
 LDI R20,0x00
STS TCNT1L,R20 ;resetting the counter to 0 for next round
RET