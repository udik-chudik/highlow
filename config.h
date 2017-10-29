#ifndef CONFIG_H
#define CONFIG_H
#define F_CPU 8000000UL
#define OVERFLOWS_PER_SECOND_TIMER1 256 /*Сколько раз переполняется таймер 1 за одну секунду*/
/*соответственно максимально возможная скорость -- это 3 м/мин достигается при F=6000*/
#define CPU_FREQURENCY 8000000
#define PWM_PORT PORTA
#define ENABLE_E PORTA4
#define ENABLE_Z PORTA5
#define ENABLE_X PORTA6
#define ENABLE_Y PORTA7

#define PORT_X PORTC
#define CLEAR_X 0b00001111
#define PORT_Y PORTC
#define CLEAR_Y 0b11110000
#define PORT_Z PORTB
#define CLEAR_Z 0b00001111
#define PORT_E PORTB
#define CLEAR_E 0b11110000


#define WAS_OVERFOLLOW 0b00000001
#define WAS_COMMENT 0b00000010

#define CRC_CHECK_ENABLED 0b00100000

#define PHASE_LENGTH 8
#define BUFFER_LENGTH 64
#define ERROR_BUFFER_OVERFOLLOW PSTR("Error: the command buffer full!\n")
#define WARNING_NOTHING_TO_DO PSTR("Warning: nothing to do!\n")
#define WARNING_UNSUPPORTED_COMMAND PSTR("Warning: unsupported command!\n")
#define ERROR_CHECKSUM_FAILED PSTR("rs\n")
#define SUCCESS_DONE PSTR("ok\n")
#define WARNING_WAITING PSTR("_\n")

#define START_MESSAGE PSTR("start\n")

#define EXTRUDER_HEATER_ON 0b10000000
#define BED_HEATER_ON 0b01000000

#define ABSOLUTE_POSITIONING 0b01000000
#define NEW_TASK 0b10000000
#define STEPS_PER_X 5
#define STEPS_PER_Y 5
#define STEPS_PER_Z 304
#define STEPS_PER_E 10
#define MAX_ALLOWED_SPEED_X 5000
#define MAX_ALLOWED_SPEED_Y 5000
#define MAX_ALLOWED_SPEED_Z 1000
#define MAX_ALLOWED_SPEED_E 1000
#define INITIAL_F 1000
#define INC_X  1
#define DEC_X -1
#define INC_Y  2
#define DEC_Y -2
#define INC_Z  3
#define DEC_Z -3
#define INC_E  4
#define DEC_E -4
struct vector
{
	float x;
	float y;
	float z;
	float e;
};

struct discret_vector
{
	long x;
	long y;
	long z;
	long e;
};

#endif
