#include "pwm.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"

unsigned char getTemperature(unsigned char adc_value){
	return 127+83*adc_value/100;
}
unsigned char iTemperature = 240;
unsigned char TemperatureWindow = 1;
unsigned char current_temperature = 0;

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

ISR(ADC_vect)
{
	
	if (iTemperature + TemperatureWindow <= getTemperature(ADCH)){
		PORTD &= 0b01111111;
	}else{
		PORTD |= 0b10000000;
	}
	current_temperature = getTemperature(ADCH);
	ADCSRA |=(1<<ADSC);
}
