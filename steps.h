/*
	16 bit Timer/Counter 1 for stepper motors phase change
*/
#include <stdint.h>
#include "config.h"
static int kappa;

void moveOn(struct vector placment, float f);
void doStep(signed char motor);