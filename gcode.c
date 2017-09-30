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
struct vector location = {0, 0, 0, 0};
struct vector placement = {0, 0, 0, 0};
*/



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
	uint8_t G = 0, M = 0;
	float x = 0, y = 0, z = 0, e = 0;
	static float f;
	uint8_t recognize_flag = 0b00000000;
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
	//	S = atoi(base + sizeof(uint8_t));
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
				case 0:	{
					break;
				}
				case 1: {
					/*
						Calculate NULL translation vector
					*/

					if (recognize_flag & 0b00010000){
						/*
							Absolute positioning
						*/
						placement.x = x;
					}else{
						placement.x = location.x;
					}
					if (recognize_flag & 0b00001000){
						/*
							Absolute positioning
						*/
						placement.y = y;
					}else{
						placement.y = location.y;
					}
					if (recognize_flag & 0b00000100){
						/*
							Absolute positioning
						*/
						placement.z = z;
					}else{
						placement.z = location.z;
					}
					if (recognize_flag & 0b00000010){
						/*
							Absolute positioning
						*/
						placement.e = e;
					}else{
						placement.e = location.e;
					}
					moveOn(f);
					break;
				}
				case 92: {
					if (recognize_flag & 0b00010000){
						/*
							Absolute positioning
						*/
						location.x = x;
					}
					if (recognize_flag & 0b00001000){
						/*
							Absolute positioning
						*/
						location.y = y;
					}
					if (recognize_flag & 0b00000100){
						/*
							Absolute positioning
						*/
						location.z = z;
					}
					if (recognize_flag & 0b00000010){
						/*
							Absolute positioning
						*/
						location.e = e;
					}
					sendStaicMessage(SUCCESS_DONE);
					break;
				}
				default: {
					sendStaicMessage(WARNING_UNSUPPORTED_COMMAND);
					break;
				}
			}

		}else if (recognize_flag & 0b01000000){
			/*
				There was M-class command
			*/
			switch(M){
				case 50:
					/*
						Enable CRC check
					*/
					state0 |= CRC_CHECK_ENABLED;
					sendStaicMessage(SUCCESS_DONE);
					break;
				case 51:
					/*
						Disable CRC check
					*/
					state0 &= ~CRC_CHECK_ENABLED;
					sendStaicMessage(SUCCESS_DONE);
					break;
				default: {
					sendStaicMessage(WARNING_UNSUPPORTED_COMMAND);
					break;
				}
			}

		}else{
			sendStaicMessage(WARNING_UNSUPPORTED_COMMAND);
		}
	}else{
		sendStaicMessage(WARNING_NOTHING_TO_DO);
	}

}
