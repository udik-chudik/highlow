#include "uart.h"
#include "config.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "gcode.h"
#include <avr/pgmspace.h>
/*
	Uart operations
*/
void sendCharter(uint8_t charter){
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = charter;
}
void sendStaicMessage(const char *string){
	while (pgm_read_byte(string)!='\0'){
		sendCharter(pgm_read_byte(string));
		string++;
	}
}
ISR(USART_RXC_vect){
	/*
		When we got new charter from UART
	*/
	/*
		Init static variables
	*/
	static uint8_t i = 0;
	static uint8_t buffer[BUFFER_LENGTH];
	static uint8_t was_comment = 0;
	/*
		Read charter
	*/
	uint8_t charter = UDR;
	if ((charter != '\n') && (charter != '\r')){
		/*
			Fill the buffer
		*/
		if (charter == ';'){
			was_comment = 1;
		}
		if (i < BUFFER_LENGTH){
			if (!was_comment){
				buffer[i] = charter;
				i++;
			}
		}else{
			sendStaicMessage(ERROR_BUFFER_OVERFOLLOW);
		}
	}else{
		/*
			The command have been arrived.
			Lets analyze it!
		*/

		AnalyzeCommand(buffer);
		/*
			Clear the buffer after analyzing
		*/
		for (i = BUFFER_LENGTH - 1; i > 0; i--){
			buffer[i] = 0;
		}
	}

}