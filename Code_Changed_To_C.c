#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU (1000000)

// Define the segments for common anode 7-segment display
#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

// Define the common pins for the displays
#define COMMON_PORT  PORTA
#define COMMON_DDR   DDRA
#define int parking
// Define the display pins
#define DISPLAY_PORT  PORTC
#define DISPLAY_DDR   DDRC

// Number of 7-segment displays
#define NUM_DISPLAYS 2

// Array to hold digits for each display
int a,b,c;
uint8_t digitsToDisplay[] = { c, b, a};

parking=200;

ISR (INT0_vect){
	led(0);
	parking--;
	_delay_ms(500);
}

ISR (INT1_vect){
	led(1);
	parking++;
	_delay_ms(500);
}

void led (int x){
	if(x==0){
		PORTB=PORTB | 1;
		_delay_ms(500);
		PORTB=0;
	}
	if(x==1){
		PORTB=PORTB | (1<<1);
		_delay_ms(500);
		PORTB=0;
	}
}
// Function to display a digit on a 7-segment display

void displayDigit(uint8_t digit) {
    switch (digit) {
        case 0: DISPLAY_PORT = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F; break;
        case 1: DISPLAY_PORT = SEG_B | SEG_C; break;
        case 2: DISPLAY_PORT = SEG_A | SEG_B | SEG_G | SEG_E | SEG_D; break;
        case 3: DISPLAY_PORT = SEG_A | SEG_B | SEG_C | SEG_D | SEG_G; break;
        case 4: DISPLAY_PORT = SEG_B | SEG_C | SEG_F | SEG_G; break;
        case 5: DISPLAY_PORT = SEG_A | SEG_C | SEG_D | SEG_F | SEG_G; break;
        case 6: DISPLAY_PORT = SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G; break;
        case 7: DISPLAY_PORT = SEG_A | SEG_B | SEG_C; break;
        case 8: DISPLAY_PORT = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G; break;
        case 9: DISPLAY_PORT = SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G; break;
        default: DISPLAY_PORT = 0; break;  // Turn off all segments for unknown digit
    }
}
void turn () {
		a = parking /100;
		b = (parking % 100) / 10;  // Tens digit
		c = parking % 10;  // Ones digit
 }
 
int main() {
	
	GICR = (1<<INT0);
	GICR =GICR | (1<<INT1);
	sei();
	
	DDRB= 0xFF;
    COMMON_DDR = 0xFF;  // Set common pins as outputs
    DISPLAY_DDR = 0xFF; // Set display pins as outputs

    while (1) {
		turn();
		if (parking =<0){
			 // Display "L," "U," "F" on each 7-segment display
            for (uint8_t display = 0; display < NUM_DISPLAYS; ++display) {
                COMMON_PORT = 1 << display; // Activate one display at a time
                switch (display) {
                    case 0: // "L"
                        DISPLAY_PORT = SEG_E | SEG_F | SEG_D ;
                        break;
                    case 1: // "L"
                        DISPLAY_PORT =SEG_E | SEG_F | SEG_D  ;
                        break;
                    case 2: // "U"
                        DISPLAY_PORT = SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
                        break;
                    case 3: // "F"
                        DISPLAY_PORT = SEG_A | SEG_E | SEG_F | SEG_G;
                        break;
		}
       else {
		    for (uint8_t display = 0; display < NUM_DISPLAYS; ++display) {
			
            COMMON_PORT = 1 << display; // Activate one display at a time

            displayDigit(digitsToDisplay[display]);

            _delay_ms(1);  // Adjust the delay based on your specific requirements
        }
	   }
    }

    return 0;
}
