;
; Demo.asm
;
; Created: 2/8/2018 04:49:22 PM
; Author : Guillermo Gálvez
;

; Replace with your application code
.equ SA = 0x0222		;Starting adress
.equ cnt = 255			;check for second loop
.equ cnt1 = 45
.equ five = 5


.org 0
RJMP start

;STUFF CAN GO HERE


start:
	clr r0					;zero r0 register
	ldi XL, low(SA)			;XL R26 = 0x--22 startadd[7:0]
	ldi XH, high(SA)		;XH R27 = 0x02-- startadd[8:15]

	ldi YL, low(0x400)		;Where to store low if div by 5
	ldi YH, high(0x400) 	;Where to store high if div by 5

	ldi ZL, low(0x600)		;Where to store low if not div by 5
	ldi ZH, high(0x600)		;Where to store low if not div by 5

	;conting registers "index"
	ldi r20, cnt		;upper found 
	
	;sum registers
	clr r0 				;R0=0
	clr r1 				;Have I been here/The Carry Bit
	clr r2 				;take up to 300 check
	clr r3 				;R3=0
	clr r5 				;R5=0

	clr r16				;R16=0
	clr r17				;R17=0
	clr r18 			;R18=0
	clr r19				;R19=0

	clr r21				;working register
	clr r22 			;working register

	clr r15				;R15=0 ZERO
	
	
	inc r1 				;did you already reset once check
	ldi r24, 5


rjmp storeLP



keepcounting:
	cp r3, r1 			;have you been here already
	brlo to300			;No then reset r0 to count another 44
	rjmp doneStoring	;Yes then you're done

	to300:
	clr r0 				;reset loop control 
	inc r3 				;set check for next time
	ldi r20, cnt1		;set to go to 300
	rjmp back 			;




		storeLP:	mov r21, XH		;load values at SA =0x02-- high into r16
					mov r22, XL		;load values at SA =0x--22 low into r17

			back:
			cp r20, r0 			;i > 254
			breq keepcounting	;if so end

 			add r21, r22		;R16 = XH + XL
 			ST X+,	r21 		;store sum of 0x0222, X -> 0x0224
 			cp r21, r15
 			breq zero 			;zero not div 5
 			mov r23, r21 		;set up r23 for divison by 5
 			rjmp divB5

 			inc r0
 			rjmp storeLP			;continue to 300 numbers

;DIVISION LOOP 		
 		divB5:	cp r23, r24		;is r23 < 5
 				brlo isLT5		;if r23 < 5 is it zero
 				sub r23, r24 	;r22 - 5
 				rjmp divB5

 		isLT5:	cp r23, r15  	;is it equal to zero 
 				breq isDiv5		;if eq zero jump

 				;NOT DIV 5/Zero not Div5
 				zero:
 				st Z+, r21 		;store not-div by 5 numb at 0x0600, Z -> 0x0604
 				add r19, r21    ;r18:r19
 				inc r0   		;next index
 				rjmp notB5
 				

 		isDiv5:	st Y+, r21 		;store div by 5 at 0x0400, Y -> 0x0400
 				add r17, r21	;r16:r17
 				inc r0 			;next index
 				rjmp by5
 				

 				by5: brcs addC5
 					 rjmp storeLP


 					addC5: add r16, r1
 							rjmp storeLP


 				notB5: brcs addCN
					   rjmp storeLP

 					 addCN: add r18, r1 
 					 		rjmp storeLP







doneStoring: RJMP doneStoring

.EXIT


 	