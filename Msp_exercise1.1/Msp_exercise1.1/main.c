/*
 * Msp_exercise1.1.c
 *
 * Created: 7/21/2024 6:00:23 PM
 * Author : USER
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL

#define disp_port PORTA
#include <util/delay.h>


char digits_ [] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
int main(void)
{DDRA |=0xff;
    /* Replace with your application code */
    while (1) 
    {
		for (int i =0;i<9;i++)
		{
			disp_port = digits_[i];
			_delay_ms(1000);
			//disp_port = 0x00;
		}
    }
}

