#include "pwm.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
/*
	8-bit Timer/Counter0 for PWM stepper control
*/
ISR(TIMER0_COMP_vect, ISR_NAKED){
	/*
		When executed -> set PWM_PIN to 0
	*/
	PWM_PORT &= ~(1<<PWM_PIN);
	reti();
}
ISR(TIMER0_OVF_vect, ISR_NAKED){
	/*
		When executed -> set PWM_PIN to 1
	*/
	PWM_PORT |= (1<<PWM_PIN);
	reti();
}