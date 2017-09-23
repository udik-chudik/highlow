/*
	Uart operations
*/
#include <stdint.h>
void sendCharter(uint8_t charter);
void sendStaicMessage(const char *string);
uint8_t checkCRC(uint8_t *input_buffer);
