/*
	16 bit Timer/Counter 1 for stepper motors phase change
*/
#include <stdint.h>
#include "config.h"
//#include "global_variables.h"
//#include "gcode.h"

/*void moveOn(struct vector endpoint, float F);*/
	
void moveOn(float F);
void doStep(signed char motor);
