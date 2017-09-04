#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "gcode.h"
#include "uart.h"
#include "steps.h"
#include "global_variables.h"
/*
	G-code analyzer
*/

void AnalyzeCommand(uint8_t *buffer){
	/*
		Parse values of G, M, S, X, Y, Z, E variables from string
	*/
	char *base;
	/*
		Init commands
	*/
	uint8_t G = 0, M = 0, S = 0;
	float x = 0, y = 0, z = 0, e = 0, f = 0;
	uint8_t recognize_flag = 0;
	/*
	recognize_flag = 0b G M S x y z e f
	*/
	/*
		Fill values, if they exist
	*/

	if ((base = strchr((const char*)buffer, 'G')) != NULL){
		G = atoi(base + sizeof(uint8_t));
		recognize_flag |= 0b10000000;
	}
	if ((base = strchr((const char*)buffer, 'M')) != NULL){
		M = atoi(base + sizeof(uint8_t));
		recognize_flag |= 0b01000000;
	}
	if ((base = strchr((const char*)buffer, 'S')) != NULL){
		S = atoi(base + sizeof(uint8_t));
		recognize_flag |= 0b00100000;
	}
	if ((base = strchr((const char*)buffer, 'X')) != NULL){
		x = atof(base + sizeof(uint8_t));
		recognize_flag |= 0b00010000;
	}
	if ((base = strchr((const char*)buffer, 'Y')) != NULL){
		y = atof(base + sizeof(uint8_t));
		recognize_flag |= 0b00001000;
	}
	if ((base = strchr((const char*)buffer, 'Z')) != NULL){
		z = atof(base + sizeof(uint8_t));
		recognize_flag |= 0b00000100;
	}
	if ((base = strchr((const char*)buffer, 'E')) != NULL){
		e = atof(base + sizeof(uint8_t));
		recognize_flag |= 0b00000010;
	}
	if ((base = strchr((const char*)buffer, 'F')) != NULL){
		f = atof(base + sizeof(uint8_t));
		recognize_flag |= 0b00000001;
	}

	if (recognize_flag){
		if (recognize_flag & 0b10000000){
			/*
				There was G-class command
			*/
			switch(G){
				case 0:
				case 1: {
					struct vector placement = location;
					/*
						Calculate NULL translation vector
					*/

					if (recognize_flag & 0b00010000){
						if (state0 & ABSOLUTE_POSITIONING){
							/*
								Relative positioning
							*/
							placement.x = location.x + x;
						}else{
							/*
								Absolute positioning
							*/
							placement.x = x;
						}
					}
					if (recognize_flag & 0b00001000){
						if (state0 & ABSOLUTE_POSITIONING){
							/*
								Relative positioning
							*/
							placement.y = location.y + y;
						}else{
							/*
								Absolute positioning
							*/
							placement.y = y;
						}
					}
					if (recognize_flag & 0b00000100){
						if (state0 & ABSOLUTE_POSITIONING){
							/*
								Relative positioning
							*/
							placement.z = location.z + z;
						}else{
							/*
								Absolute positioning
							*/
							placement.z = z;
						}
					}
					if (recognize_flag & 0b00000010){
						if (state0 & ABSOLUTE_POSITIONING){
							/*
								Relative positioning
							*/
							placement.e = location.e + e;
						}else{
							/*
								Absolute positioning
							*/
							placement.e = e;
						}
					}
					moveOn(placement, f);
					break;
				}
				default: {
					sendStaicMessage(WARNING_UNSUPPORTED_COMMAND);
					break;
				}
			}

		}
	}else{
		sendStaicMessage(WARNING_NOTHING_TO_DO);
	}

}