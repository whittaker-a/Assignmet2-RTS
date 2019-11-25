/*
 * commands.h
 *
 *  This code is intended to be used for:
 *  setting the time,
 *  reading the time,
 *  setting an alarm,
 *  buzzing the alarm,
 *  Setting the date,
 *  reading the date.
 *
 *  Several constants are defined here as well that
 *  regard date and time.
 *
 *  Created on: Oct 1, 2019
 *      Author: AWhit
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "queueStruct.h"
#include "UART0.h"



//set max size for input command to 8
//set max size for argument to 16
#define COMMAND_SIZE    5
#define ARGUMENT_SIZE   16

#define ERROR   -1

#define WHITE_SPACE ' '
#define ENTER_KEY   0x0D

unsigned char compareStrings(char *source, char *dest, short length);
void tokenize_time(char *entry, short* time);
short AtoI(char *integer, const short str_length);
//void get_time();
//void set_time(const char *input_time);
void process_command(QUEUE *p);

#endif /* COMMANDS_H_ */
