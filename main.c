#include "init.h"
#include <avr/interrupt.h>

int main(void)
{
	/* code */
	initDevice();
	sei();
	while(1);
	return 0;
}