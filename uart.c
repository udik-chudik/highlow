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
uint8_t checkCRC(uint8_t *input_buffer){

	uint16_t CRC = 0;
	int i = 0;

	while (input_buffer[i] != '*'){
		CRC = (CRC << 3) + (uint16_t) input_buffer[i];
		CRC = (CRC << 3) + (uint16_t) input_buffer[i];
		CRC = (CRC ^ (CRC >> 8));
		//if (input_buffer[i] == 'R') PORTC |= (1<<PC0);
		//sendCharter((uint8_t)CRC);
		i++;
	}
	i++;
	//sendCharter((uint8_t)CRC);
	if (input_buffer[i] == (uint8_t)CRC){
		return 1;
	}else{
		return 0;
	}

}
ISR(USART_RXC_vect){
	/*
		When we got new charter from UART
	*/
	/*
		Init static variables
	*/
	/*static uint8_t k = 0;*/
	static uint8_t buffer[BUFFER_LENGTH];
	static uint8_t bufferSwap[BUFFER_LENGTH];
	static uint8_t was_comment = 0;
	static int k = 0;
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
		if (k < BUFFER_LENGTH){
			if (!was_comment){
				buffer[k] = charter;
				k++;
			}
		}else{
			for (k = BUFFER_LENGTH - 1; k >= 0; k--){
				bufferSwap[k]=buffer[k];
				buffer[k] = 0;
			}
			k=0;
			sendStaicMessage(ERROR_BUFFER_OVERFOLLOW);
		}
	}else{
		buffer[k] = charter;
		/*
			Clear the buffer before analyzing
		*/
		for (k = BUFFER_LENGTH - 1; k >= 0; k--){
			bufferSwap[k]=buffer[k];
			buffer[k] = 0;
		}
		k=0;
		/*
			The command have been arrived.
			Lets analyze it!
		*/
		if (checkCRC(bufferSwap) || was_comment){ //Кастыль
			AnalyzeCommand(bufferSwap);
		}else{
			sendStaicMessage(ERROR_CHECKSUM_FAILED);
		}
		was_comment = 0;



	}

}
