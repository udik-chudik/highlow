#include "init.h"
#include <avr/interrupt.h>
#include "uart.h"
#include "config.h"
#include <avr/pgmspace.h>


int main(void)
{
	/* code */
	initDevice();
	sendStaicMessage(START_MESSAGE);
	sei();
	while(1);
	return 0;
}
