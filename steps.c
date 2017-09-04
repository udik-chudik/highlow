#include "steps.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <math.h>
#include "global_variables.h"
#include "uart.h"


const char DRIVE_PHASES[] PROGMEM = {0b10000000,0b11000000,0b01000000,0b01100000,0b00100000,0b00110000,0b00010000,0b10010000};

static struct discret_vector translation_discret;
static int translation_discret_length;
static struct discret_vector progress;

ISR(TIMER1_OVF_vect){
	static int j = 0;
	static int i = 0;
	if (state0 & NEW_TASK){
		if (j < kappa){
			j++;
		}else{
			j = 0;
			if (translation_discret.x > 0){
				if (progress.x < i*translation_discret.x/translation_discret_length){
					progress.x = i*translation_discret.x/translation_discret_length;
					location.x += 1.0/STEPS_PER_X;
					doStep(INC_X);
				}
			}else{
				if (progress.x > i*translation_discret.x/translation_discret_length){
					progress.x = i*translation_discret.x/translation_discret_length;
					location.x -= 1.0/STEPS_PER_X;
					doStep(DEC_X);
				}
			}

			if (translation_discret.y > 0){
				if (progress.y < i*translation_discret.y/translation_discret_length){
					progress.y = i*translation_discret.y/translation_discret_length;
					location.y += 1.0/STEPS_PER_Y;
					doStep(INC_Y);
				}
			}else{
				if (progress.y > i*translation_discret.y/translation_discret_length){
					progress.y = i*translation_discret.y/translation_discret_length;
					location.y -= 1.0/STEPS_PER_Y;
					doStep(DEC_Y);
				}
			}

			if (translation_discret.z > 0){
				if (progress.z < i*translation_discret.z/translation_discret_length){
					progress.z = i*translation_discret.z/translation_discret_length;
					location.z += 1.0/STEPS_PER_Z;
					doStep(INC_Z);
				}
			}else{
				if (progress.z > i*translation_discret.z/translation_discret_length){
					progress.z = i*translation_discret.z/translation_discret_length;
					location.z -= 1.0/STEPS_PER_Z;
					doStep(DEC_Z);
				}
			}

			if (translation_discret.e > 0){
				if (progress.e < i*translation_discret.e/translation_discret_length){
					progress.e = i*translation_discret.e/translation_discret_length;
					location.e += 1.0/STEPS_PER_E;
					doStep(INC_E);
				}
			}else{
				if (progress.e > i*translation_discret.e/translation_discret_length){
					progress.e = i*translation_discret.e/translation_discret_length;
					location.e -= 1.0/STEPS_PER_E;
					doStep(DEC_E);
				}
			}

			if (i < translation_discret_length){
				i++;
			}else{
				/*
					When task has been complete
				*/
				state0 &= ~NEW_TASK;
				i = 0;
				sendStaicMessage(SUCCESS_DONE);
			}
		}
		/*
		PORT_X &= CLEAR_X;
		PORT_X |= pgm_read_byte(driveX + j);
		if ( j == PHASE_LENGTH - 1){
			j = 0;
		}else{
			j++;
		}
		*/
	}
}

void moveOn(struct vector endpoint, float F){
	/*
		Calculate translation vector
	*/
	struct vector translation;
	
	translation.x = endpoint.x - location.x;
	translation.y = endpoint.y - location.y;
	translation.z = endpoint.z - location.z;
	translation.e = endpoint.e - location.e;

	translation_discret.x = translation.x*STEPS_PER_X;
	translation_discret.y = translation.y*STEPS_PER_Y;
	translation_discret.z = translation.z*STEPS_PER_Z;
	translation_discret.e = translation.e*STEPS_PER_E;

	/*
		Calcutale length of vectors
	*/

	float translation_length = sqrt(
		translation.x*translation.x +
		translation.y*translation.y + 
		translation.z*translation.z + 
		translation.e*translation.e
	);

	int translation_discret_length = sqrt(
		translation_discret.x*translation_discret.x +
		translation_discret.y*translation_discret.y + 
		translation_discret.z*translation_discret.z + 
		translation_discret.e*translation_discret.e
	);

	/*
		Calculate speed if F changed
	*/
	if (F > 0){

		/*
			Check for speed limit
		*/

		if (F*abs(translation.x)/translation_length > MAX_ALLOWED_SPEED_X){
			F = MAX_ALLOWED_SPEED_X*translation_length/abs(translation.x);
		}
		if (F*abs(translation.y)/translation_length > MAX_ALLOWED_SPEED_Y){
			F = MAX_ALLOWED_SPEED_Y*translation_length/abs(translation.y);
		}
		if (F*abs(translation.z)/translation_length > MAX_ALLOWED_SPEED_Z){
			F = MAX_ALLOWED_SPEED_Z*translation_length/abs(translation.z);
		}
		if (F*abs(translation.e)/translation_length > MAX_ALLOWED_SPEED_E){
			F = MAX_ALLOWED_SPEED_E*translation_length/abs(translation.e);
		}
		/*
			Calculate new kappa
		*/
		kappa = translation_length*CPU_FREQURENCY*60/(F*TIMER1_DIVIDER*translation_discret_length);
	}
	/*
		Prepare variables
	*/
	progress.x = 0;
	progress.y = 0;
	progress.z = 0;
	progress.e = 0;
	/*
		Let's move!
	*/
	state0 |= NEW_TASK;
}

void doStep(signed char motor){

	static uint8_t phases[] = {0, 0, 0, 0};
	int next_phase = 0;

	if (motor > 0){
		next_phase = phases[motor - 1];
		if (next_phase == PHASE_LENGTH - 1){
			next_phase = 0;
		}else{
			next_phase++;
		}
		phases[motor - 1] = next_phase;
	}else{
		next_phase = phases[-1 - motor];
		if (next_phase == PHASE_LENGTH - 1){
			next_phase = 0;
		}else{
			next_phase++;
		}
		phases[-1 - motor] = next_phase;
	}

	switch(motor){
		case INC_X:
		case DEC_X: {
			PORT_X &= CLEAR_X;
			PORT_X |= pgm_read_byte(DRIVE_PHASES + next_phase);
		}
		case INC_Y:
		case DEC_Y: {
			PORT_Y &= CLEAR_Y;
			PORT_Y |= pgm_read_byte(DRIVE_PHASES + next_phase);
		}
		case INC_Z:
		case DEC_Z: {
			PORT_Z &= CLEAR_Z;
			PORT_Z |= pgm_read_byte(DRIVE_PHASES + next_phase);
		}
		case INC_E:
		case DEC_E: {
			PORT_E &= CLEAR_E;
			PORT_E |= pgm_read_byte(DRIVE_PHASES + next_phase);
		}
	}

}