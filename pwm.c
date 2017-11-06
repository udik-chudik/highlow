#include "pwm.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "global_variables.h"

unsigned char getTemperature(unsigned char adc_value){
	return 127+83*adc_value/100;
}

unsigned char getTemperature_bed(unsigned char adc_value){
	return 83*adc_value/10;
}

unsigned char iTemperature = 240;
unsigned char TemperatureWindow = 1;


unsigned char iTemperature_bed = 110;
unsigned char TemperatureWindow_bed = 1;


/*
	8-bit Timer/Counter0 for PWM stepper control
*/
ISR(TIMER0_COMP_vect, ISR_NAKED){
	/*
		When executed -> set PWM_PIN to 0
	*/

	if (state0 & NEW_TASK){
		PWM_PORT |= (1<<ENABLE_X);
		PWM_PORT |= (1<<ENABLE_Y);
		PWM_PORT |= (1<<ENABLE_Z);
		PWM_PORT |= (1<<ENABLE_E);

	}else{
		PWM_PORT &= ~(1<<ENABLE_X);
		PWM_PORT &= ~(1<<ENABLE_Y);
		PWM_PORT &= ~(1<<ENABLE_Z);
		//PWM_PORT &= ~(1<<ENABLE_E);
	}

	reti();
}
ISR(TIMER0_OVF_vect, ISR_NAKED){
	/*
		When executed -> set PWM_PIN to 1
	*/
/*
	PWM_PORT |= (1<<ENABLE_X);
	PWM_PORT |= (1<<ENABLE_Y);
	PWM_PORT |= (1<<ENABLE_Z);
	PWM_PORT |= (1<<ENABLE_E);

*/
	reti();
}

ISR(ADC_vect)
{

	if (ADMUX & (1<<MUX0)){
		/*
			BED Result
		*/
		if (iTemperature_bed + TemperatureWindow_bed <= getTemperature_bed(ADCH)){
			PORTD &= ~BED_HEATER_ON;
		}else{
			PORTD |= BED_HEATER_ON;
		}
		current_temperature_bed = getTemperature_bed(ADCH);

		ADMUX ^= (1<<MUX0);
	}else{
		/*
			Extruder result
		*/
		if (iTemperature + TemperatureWindow <= getTemperature(ADCH)){
			PORTD &= ~EXTRUDER_HEATER_ON;
		}else{
			PORTD |= EXTRUDER_HEATER_ON;
		}
		current_temperature = getTemperature(ADCH);
		
		ADMUX ^= (1<<MUX0);

	}	
	
	ADCSRA |= (1<<ADSC);
}
