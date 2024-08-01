/*
 * GccApplication1.c
 *
 * Created: 7/22/2024 10:28:23 AM
 * Author : USER
 */ 

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/delay.h> 
#include <stdio.h>

char display[] = {0x3F ,0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67}; 
int SEG1 = 4; 
int SEG2 = 5; 
int delayvalue = 500; // Reduced delay value for higher refresh rate
int i = 0; 
int j = 0; 

/* A function to act as a persistence of vision delay */
void vision_persist(){ 
    int delay = delayvalue; 
    while (0 < delay){
        _delay_us(1); 
        --delay; 
    } 
} 

/* A function to reload the timer initial value after the timer overflows */ 
void sevseg_refreshDisplay(){ 
    TCNT1L = 0x00; /* Load TCNT1, count for a shorter interval */ 
    TCNT1H = 0x00; 
    TCCR1B |= (1<<CS10); /* Start timer1 with no prescaler for higher frequency */ 
    TIMSK1 |= (1<<TOIE1); /* Enable Timer1 overflow interrupts */ 
} 

void displayLED(int input){ 
    /* Function to display a number in the LED */ 
    char BH = (PORTB & 0xF0); 
    char BL = (display[input] & 0x0F); 
    PORTB = (BH | BL);
    char DL = (PORTD & 0x0F);
    char DH = (display[input] & 0xF0);
    PORTD = (DH | DL); 
	
} 

ISR(TIMER1_OVF_vect){ 
  /* Interrupt Service Routine for Timer1 */
  PORTB ^= (1<<SEG1); /* Toggle SEG1 */
  PORTB ^= (1<<SEG2); /* Toggle SEG2 */
  displayLED(i);
  vision_persist();
  PORTB ^= (1<<SEG1); /* Toggle SEG1 */
  PORTB ^= (1<<SEG2); /* Toggle SEG2 */
  displayLED(j);
  vision_persist();
  TCNT1 = 0x00; // Reset Timer1 counter
  TIFR1 |= (1<<TOV1); // Clear Timer1 overflow flag
} 

void initADC() {
    ADMUX = (1<<REFS0); // Select Vref=AVcc
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // Enable ADC and set prescaler to 128
}

uint16_t readADC(uint8_t ch) {
    ch &= 0b00000111; // Select ADC channel ch must be 0-7
    ADMUX = (ADMUX & 0xF8) | ch; // Clear the older channel that was read
    ADCSRA |= (1<<ADSC); // Start single conversion
    while(ADCSRA & (1<<ADSC)); // Wait for conversion to complete
    return (ADC);
}

int main(void){ 
    int count = 0; 
    DDRB |= 0xFF; 
    DDRD |= 0xFF; 
    PORTB |= (1<<SEG1); 
    /***********************************************************************/ 
    /* Interrupt Setup */ 
    sevseg_refreshDisplay();
    sei(); /* Enable Global Interrupt */ 
    initADC(); // Initialize ADC
    /***********************************************************************/
    while (1) {
        uint16_t adcValue = readADC(0); // Read ADC value from channel 0
        count = (adcValue * 99L) / (1023); // Map ADC value to range 0-99
        i = count / 10; // Tens place
        j = count % 10; // Units place
        // Debug: Print ADC value and mapped count
        printf("ADC Value: %d, Mapped Count: %d\n", adcValue, count);
		
		displayLED(i); // Display tens digit
		vision_persist(); // Delay for persistence of vision
		displayLED(j); // Display units digit
		vision_persist(); // Delay for persistence of vision


        _delay_ms(100); 
		adcValue =0;
		
    } 
}