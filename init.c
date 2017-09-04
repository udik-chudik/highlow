#include "init.h"
#include <avr/io.h>
#include "steps.h"
void initDevice(void){
	/*
	Configure ports
	*/
	/*
		PORTC - 0bXXXX0000 - high 4 bist X-axis stepper motor
		PORTC - 0b0000YYYY - low 4 bits Y-axis stepper motor
	*/
	DDRC = 0xFF;
	/*
		PORTA - 0b0000000A - ADC0 - input for extruder temperature control
	*/
	DDRA = 0xFE;
	/*
		PORTB - 0bZZZZ0000 - high 4 bits Z-axis stepper motor
		PORTB - 0b0000EEEE - low 4 bits E-axis stepper motor
	*/
	DDRB = 0xFF;
	/*
		PORTD - 0bE0000000 - 7th bit out fot extruder heater
	*/
	DDRD = (1<<PORTD7);
	/*
		Internal timers initialization
	*/
	/*
		8-bit Timer/Counter0 for PWM stepper control
		Fast PWM mode:
			WGM01 -> 1
			WGM00 -> 1
		Clock select: internal, OSC/1024:
			CS00 -> 1
			CS01 -> 0
			CS02 -> 1

	*/
	TCCR0 = (1<<WGM01)|(1<<WGM00)|(1<<CS02)|(1<<CS00);
	/*
		OCR0 - compare value:
			1 - small pulse width
			200 - big pulse width
	*/
	OCR0 = 20;
	/*
		Enable interrupt on overfolow and compare match
	*/
	TIMSK |= (1<<OCIE0)|(1<<TOIE0);
	/*
		16 bit Timer/Counter 1 for stepper motors phase change
		Fast PWM mode with TOP in OCR1A:
			WGM10 -> 1
			WGM11 -> 1
			WGM12 -> 1
			WGM13 -> 1
		Clock select: internal, OSC/256
			CS10 -> 0
			CS11 -> 0
			CS12 -> 1

	*/
	TCCR1A = (1<<WGM11)|(1<<WGM10);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS12);

	/*
		Compare value:
			1 - fast
			65535 - slow
	*/

	OCR1A = 30000;
	/*
		Output Compare A Overfolow Interrupt Enable
	*/
	TIMSK |= (1<<TOIE1);
	/*
		USART Initialization
		UCSRB:
			EXCIE -> rx complete interrupt enable
			TXEN, RXEN -> enable transmitter end receiver
		UCSRC:
			URSEL -> accessing to UCSRC nor UBRRH
			UCSZ0 -> 3 -> 8 bit per symbol
		UBRR -> baud rate 9600
	*/

	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
	UBRRH = 0;
	UBRRL = 51;
	/*
		Initial speed for drives
	*/
	kappa = CPU_FREQURENCY*60/(INITIAL_F*TIMER1_DIVIDER);
}