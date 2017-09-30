/*
	Global variables
*/
#include "config.h"
#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
extern struct vector location;
extern struct vector placement;

extern uint8_t state0;

/*
	state0 - control byte
	0b
	1 - 0 - we can't move, 1 - we can move
	2 - 0 - absolute, 1 - relative positioning
	3 - 0 - no CRS check, 1 - CRC check
	4
	5
	6
	7
	8
*/
#endif
