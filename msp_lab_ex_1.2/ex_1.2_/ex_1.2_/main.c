/*
 * ex_1.2_.c
 *
 * Created: 7/21/2024 6:23:51 PM
 * Author : USER
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#define disp_port PORTC
#define digit_port PORTD
void delay1();

char digits [] = {0x3F ,0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
	int num = 0;
int counter = 0;
int main(void)
{	DDRC |=(0xff);
	DDRD |=(0xff);
	
	PORTD |= (1<<PD0);
	PORTD |= (1<<PD1);
    /* Replace with your application code */
    while (1) 
    {
		for (int i = 0; i<9;i++)
		{
			counter = 0;
			while(counter <6250){
				disp_port = digits[i];
				PORTD &=(~1<<PD1);
				delay1();
				PORTD |= (1<<PD1);
				
				delay1();
				disp_port = 0x00;
				digit_port = digits[num];
				PORTD &=(~1<<PD0);
				delay1();
				PORTD |= (1<<PD0);
				disp_port = 0x00;
				delay1();
				counter++;
			}
			
			
		}
		num++;
		if (num>9)
		{
			num=0;
			
		}
    }
	return 0;
}

void delay1(){
	
	TCNT0 = 0x5F; /* Load TCNT0 with the start value*/ 
	TCCR0 = 0x01; /* Timer0, normal mode, no pre-scalar */ 
	while((TIFR&0x01)==0); /* Wait for TOV0 to roll over, i.e. overflow */ 
	TCCR0 = 0; /* set TCCR0 to 0 to stop the timer */ 
	TIFR = 0x1;  /* Clear TOV0 flag*/
	
}
