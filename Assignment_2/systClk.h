/*
 *  This Header File corresponds to the system clock
 *
 *  Multiple values are defined her as well as the structs for date and time
 *  time is used for the alarm and in the date struct
 *  date is used for the system clock.
 *
 *
 *  Created on: Oct 6, 2019
 *      Author: AWhit
 */

#ifndef SYSTCLK_H_
#define SYSTCLK_H_

#include "queueStruct.h"


extern QUEUE clockQueue;

#define NUMBER_OF_MONTHS 12
#define NOT_LEAP_YEAR    1
#define LEAP_YEAR_PERIOD 4

#define SECONDS_IN_MINUTE 60
#define MINUTE_IN_HOUR    60
#define HOUR_IN_DAY       24
#define TENS_IN_SECOND    10
#define MONTHS_IN_YEAR    12

#define COMMAND_DELIMITER ':'
#define DATE_DELIMITER    "-"
#define MESSAGE_SIZE      32

#define LENGTH_MONTH_NAME   3
#define NUM_TIME_ELEMENTS   3
#define NUM_DATE_ELEMENTS   3
#define NUM_DIGITS_TIME_ELEMENT 2
#define NUM_DIGITS_YEAR         4
#define VOID_VALUE             -1


#define INITIAL_YEAR 2019
#define INITIAL_MNTH 8
#define MAX_YEAR     9999
#define LOW_YEAR     0
#define FIRST_DAY    1

short tick();

void init_AlarmClock();
void init_SystemClock();

void get_time();
void set_time(char *input_time);
void get_date();
void set_date(char *input_date);
void clear_alarm();
void alarm(char *input_alarm);


/*
 * Definition of typedef struct time
 * and typedef struct date
 */
typedef struct time_{
    short tenth;
    short second;
    short minute;
    short hour;
}TIME_;

typedef struct date_{

    void (*incrementTime)();
    void (*incrementDate)();

    TIME_ time;

    short day;
    short month;
    short year;

} DATE_;


#endif /* SYSTCLK_H_ */
