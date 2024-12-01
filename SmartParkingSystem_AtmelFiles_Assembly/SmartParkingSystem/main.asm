;
; SmartParkingSystem.asm
;
; Created: 12/5/2023 8:05:30 PM
; Author :  sharfi
;


; Replace with your application code
.include "m32def.inc"
start:

 ldi r16, high(RAMEND)
    out SPH, r16     ; Load high part of RAMEND into SPH register
    ldi r16, low(RAMEND)
    out SPL, r16     ; Load low part of RAMEND into SPL register
	ldi r20,0
	ldi r30,2
	ldi r29,0
	ldi r28,0
	ldi r16, 0xFF		; Set all bits (DDRA) to 1 (output)
	ldi r17, 0b11000000	; Set  bits (DDRA) to 0 (input) n 1 (output)
	;ldi r30,200			; parking starts will 200 avalible seats
	call display
	out DDRB, r16
	out DDRA, r16 
	out DDRC, r16
	out DDRD, r17
		;initilze display to be 200

TEST_SENSOR:

	clr	 r20	
	out	 PORTD, r20						; clear the z flag
		call delay
	clz
	;ldi	 r18,0b00001100	; initlize both sensors are on
	in	 r18 , PIND	; read input from sensors
	;ldi	 r26,0b01000001  ; Load a custom value into register R16
	;out	 PORTD, r26  27
	cpi	 r18,0
	BREQ TEST_SENSOR	; if both sensors were turned on just go back and do nothing to the count
	clz
	mov	 r19 ,r18		; put in the same value for both registers r18 for cars that enters r19 for cars exiting 
	cbr	 r18,0b00001000	; sets the bit for sensor 2 to low  
	BREQ minus		; checks if both bits 7 , 0 are low means sensor 1 activated (active low) 
	clz
	cbr	 r19,0b00000100		; sets the bit for sensor 1 to low  
	BREQ plus	; checks if both bits 7 , 0 are low means sensor 2 activated (active low) 
rjmp TEST_SENSOR ; contiune testing untill one of the sensors detects a car





	PARKING_FULL:
	sbr r23,0b00010000
	out porta,r23
	rjmp TEST_SENSOR


	END :
    rjmp END
	

	plus: ;code that will decrease the count
	inc r28   
	cpi r28, 10    
	BRGE plus1     
	clc
	cplus1:
	adc r29,r20
	cpi r29 , 10
	BRGE plus2
	clc
	cplus2:
	adc r30,r20
	call display
	ldi r20,0b01000000	
	out portd, r20	
	rjmp TEST_SENSOR 


	plus1:
	ldi r28,0
	sec
	rjmp cplus1

	plus2:
	ldi r29,0
	sec
	rjmp cplus2






	minus: ;code that will decrease the count
	cbr r23,0b00010000
	out porta,r23
	clc
	subi r28, 1     ; Subtract 1 from r28
	cbr	 r28, 0b10000000
	cpi r28, 10     ; Compare r28 with 10
	BRGE minus1     ; Branch to RESET1 if r28 is greater than or equal to 10
	clc
	continue1:
	sbci r29,0
	cbr	 r29, 0b10000000
	cpi r29 , 10
	BRGE minus2
	clc
	continue2:
	sbci r30,0
	tst r29  
	breq TEST_ZERO
	continue3:
	call display
	ldi r20,0b10000000	
	out portd, r20	
	rjmp TEST_SENSOR 




		TEST_ZERO:
	 tst r28    
	 brne continue3
	 brne continue3 ;if all equal to zero go to full parking
	 rjmp PARKING_FULL

	

		minus1:
	 ldi r28, 9
	 sec
	 rjmp continue1

		minus2:
	 ldi r29, 9
	 sec
	 rjmp continue2


	 Display: 
	 ;a r30 b r29 c r28
	 out PORTA,r30
	 out PORTB,r29
	 out PORTC,r28
	 ret
	 ; Delay subroutine
	 delay:
	 ldi r22, 0xF ; Set the delay counter
	 delay_loop:
	 dec r22       ; Decrement the counter
	 brne delay_loop  ; Repeat the loop until the counter becomes zero
	 ret

	 