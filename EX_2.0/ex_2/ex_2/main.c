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

char display[] = {0x3F ,0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67}; 
int SEG1 = 4; 
int SEG2 = 5; 
int delayvalue = 500; 
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
    sevseg_refreshDisplay(); 
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
    /***********************************************************************/
    while (1) {
        /* Change this code segment to achieve a 10s countdown */ 
        if(count < 10){
            i = count; 
            j = 9-count;
            count++; 
        }
        if(count == 10){ 
            count = 0; 
        }
        _delay_ms(500); 
    } 
}