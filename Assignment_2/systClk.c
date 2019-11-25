/*
 * Defines the functions which control the system clock
 * The systemClock is a static global value, this code is the entire scope of the clock
 * this adds protection as only these functions can change the time
 *
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "systClk.h"
#include "UART0.h"
#include "commands.h"

enum time_t {HOUR, MINUTE, SECOND};
enum date_t {DAY, MONTH, YEAR};

// The system clock -> stores date and time
static DATE_ *systemClock;
//the system alarm -> stores time
static TIME_ *alarmClock;

//The Queue which holds clock ticks
QUEUE clockQueue = {};

//static TIME_ systemAlarm;
//Standard loop up table, pass integer month and leap year status, return num days in month
unsigned char lookup_days_by_month(unsigned char month, unsigned char notleapYear){
    if(notleapYear){
        notleapYear=NOT_LEAP_YEAR;
    }
    static const unsigned char days[NUMBER_OF_MONTHS][2] =
    {
       {31,31}, //jan
       {29,28}, //feb
       {31,31}, //mar
       {30,30}, //apr
       {31,31}, //may
       {30,30}, //jun
       {31,31}, //jul
       {31,31}, //aug
       {30,30}, //sep
       {31,31}, //oct
       {30,30}, //nov
       {31,31}, //dec
    };
    return days[month][notleapYear];
}

/*
 * Standard look-up table
 * Pass is month number and a char
 * Store month name in char
 */
void lookup_month_by_day(unsigned char month, char* name ){
    static const char* month_names[NUMBER_OF_MONTHS]=
    {
        "JAN",
        "FEB",
        "MAR",
        "APR",
        "MAY",
        "JUN",
        "JUL",
        "AUG",
        "SEP",
        "OCT",
        "NOV",
        "DEC"
    };
    strcpy(name,month_names[month]);
}

/*
 * Both alarmClock and systemClock
 * are static globals within this scope
 *
 * This function returns TRUE if they have the same time
 * and FALSE if the time is different.
 *
 * Because a person may set an alarm at night for the morning
 * The condition of >= is not used
 *
 */

void check_Alarm(){
    if(systemClock->time.second == alarmClock->second &&
            systemClock->time.minute == alarmClock->minute &&
            systemClock->time.hour == alarmClock->hour)
    {
        writeStringToConsole("!*!*\tALARM\t*!*!");
    }
}

/*
 * Increments the date when time.hour reaches 24.
 * stored in static DATE_ element
 */
void incrementDate_(){
    systemClock->day++;
    if(systemClock->day >= lookup_days_by_month(systemClock->month,systemClock->year%LEAP_YEAR_PERIOD)){
        systemClock->day = 1;
        systemClock->month++;
        if(systemClock->month >= MONTHS_IN_YEAR){
            systemClock->month = 0;
            systemClock->year++;
        }
    }
}

/*
 * Increments time every instance a clock tick is dequeued
 * checks the alarm every second
 * calls incrementDate once a day
 */
void incrementTime_(){
    systemClock->time.tenth++;
    if(systemClock->time.tenth >= TENS_IN_SECOND){
        check_Alarm();
        systemClock->time.second++;
        systemClock->time.tenth = 0;
        if(systemClock->time.second >= SECONDS_IN_MINUTE){
            systemClock->time.second = 0;
            systemClock->time.minute++;
            if(systemClock->time.minute >= MINUTE_IN_HOUR ){
                systemClock->time.minute = 0;
                systemClock->time.hour++;
                if(systemClock->time.hour >= HOUR_IN_DAY){
                    systemClock->time.hour = 0;
                    systemClock->incrementDate();
                }
            }
        }
    }
}

/*
 * Sets the values of the static global alarmClock
 * When this time reached, alarm even occurs
 */
void setAlarm(short second, short minute, short hour){
    alarmClock->second = second;
    alarmClock->minute = minute;
    alarmClock->hour   = hour;
}
/*
 * allocates memory and initializes the alarm to null
 * the alarm must first be set by the user
 *
 */
void init_AlarmClock(){
    alarmClock = malloc(sizeof(TIME_));
    setAlarm(VOID_VALUE,VOID_VALUE,VOID_VALUE);
}

/*
 * alloates memory for the system clock
 * initializes date and time to default
 * month int value corresponds to lookup_tabe
 */
void init_SystemClock(){

    systemClock = malloc(sizeof(DATE_));

    systemClock->day = FIRST_DAY;
    systemClock->month = INITIAL_MNTH;
    systemClock->year = INITIAL_YEAR;
    systemClock->time.tenth = 0;
    systemClock->time.second = 0;
    systemClock->time.minute = 0;
    systemClock->time.hour = 0;

    //these are easily called above and handle date and time
    systemClock->incrementDate=incrementDate_;
    systemClock->incrementTime=incrementTime_;
}
/*
 * This function is passed a value and an enum describing whether it is
 * HOUR, MINUTE, SECOND and verifies if it meets the criteria of valid entry
 * Criteria is: Seconds between 0 and 59
 *              Minutes between 0 and 59
 *              Hour is between 0 and 59
 */
short time_isValid(short val,enum time_t time_element){
    switch(time_element){
    case HOUR:
        if(val >= 0 && val < HOUR_IN_DAY)
            return TRUE;
        break;
    case MINUTE:
        if(val >=0 && val<MINUTE_IN_HOUR)
            return TRUE;
        break;
    case SECOND:
        if(val >=0 && val<SECONDS_IN_MINUTE)
            return TRUE;
        break;
    }
    return FALSE;
}


/*
 * This function returns the value of the systemClock.time static global
 * system only has 1 system clock
 * format - hour:minute:second
 */
void get_time(){
    //needs to make a string of hour, minute, second
    char out[MESSAGE_SIZE];
    sprintf(out,"%.2i:%.2i:%.2i", systemClock->time.hour,systemClock->time.minute,systemClock->time.second);
    writeStringToConsole(out);
}


/*
 * Takes in the time argument and verifies hour value.
 * if valid, checks for minute value
 *           if minute value, pass
 *           else
 */
void get_time_value(char *input_time, short *time){

    char * temp = malloc(2*sizeof(char));
    short index = 0;

    char str[NUM_DIGITS_TIME_ELEMENT];
    short str_counter;
    while(index < NUM_TIME_ELEMENTS){
        index++;
        *time=0;
        if(*input_time == COMMAND_DELIMITER){
            //do nothing
        }
        else if(isdigit(*input_time)){

            //reset string counter
            str_counter = 0;

            //creates a string of of digits
            while(isdigit(*input_time) && str_counter<NUM_DIGITS_TIME_ELEMENT){
                str[str_counter] = *input_time;
                input_time++;
                str_counter++;
            }

            //this value must be either time delimiter : or Null representing end of string
            if(*input_time != COMMAND_DELIMITER && *input_time != NULL)
                //time entered larger than max value or not right
                *time = ERROR;
            else
                *time = AtoI(str,str_counter);
        }
        else if(*input_time != NULL){
            //Not a ':' or a number -> something's up
            *time = ERROR;
        }
        //if the input string is empty but more time data is to be realized
        //used to handle inputs such as time hh
        if(*input_time != NULL)
            input_time++;
        time++;
    }
}

/*
 * Takes in argument passed by user
 * validates argument contains legal values for hour, minute, and second
 * if all are valid, store in systemClock.time
 * else, do not change data and warn user
 */
void set_time(char *input_time){

    short time[NUM_TIME_ELEMENTS];

    get_time_value(input_time, &time);

    if(time_isValid(time[HOUR],HOUR) && time_isValid(time[MINUTE],MINUTE) && time_isValid(time[SECOND], SECOND)){
        systemClock->time.hour = time[HOUR];
        systemClock->time.minute = time[MINUTE];
        systemClock->time.second = time[SECOND];

        get_time();
    }
    else{
        ErrorMessage();
    }
}


/*
 * return the value of systemClock date to the user
 * outputted in the form day-MON-year
 */
void get_date(){
    char out[MESSAGE_SIZE];
    char month[LENGTH_MONTH_NAME] ;
    //get's name of month to write to user
    lookup_month_by_day(systemClock->month,month);
    //creates a string stored in month
    sprintf(out,"%.2i-%s-%.4i", systemClock->day,month,systemClock->year);
    writeStringToConsole(out);
}
/*
 * Validates that the entered year value is within bounds of 0-9999
 */
short year_isValid(short year){
    if(year>=LOW_YEAR && year <= MAX_YEAR){
        return TRUE;
    }
    return FALSE;
}
/*
 * Recieves a string containing user input for date argument
 * convert each element of input to corresponding day, month, year, data
 * if valid, store in systemClock
 * else, store no data and warn user
 */
void set_date(char *input_date){
    short date[NUM_DATE_ELEMENTS];
    date[DAY]   = AtoI(strtok(input_date,DATE_DELIMITER ), NUM_DIGITS_TIME_ELEMENT);


    char temp[LENGTH_MONTH_NAME];
    char test_month[LENGTH_MONTH_NAME];
    short found = FALSE;
    //get the name of the user entered month, store in temp
    strcpy(temp,strtok(NULL,DATE_DELIMITER ));
    int i = 0;
    while(i<MONTHS_IN_YEAR&&found == FALSE){
        lookup_month_by_day(i,test_month);
        if(compareStrings(test_month, temp, LENGTH_MONTH_NAME)){
            found =TRUE;
        }
        i++;
    }

    if(found){
        date[MONTH] = i-1;
    }
    else{
        ErrorMessage();
        return;
    }
    date[YEAR]  = AtoI(strtok(NULL,DATE_DELIMITER ), NUM_DIGITS_YEAR);

    //insures dates are valid
    if(!(date[DAY] < lookup_days_by_month(date[MONTH],date[YEAR]%LEAP_YEAR_PERIOD))){
        ErrorMessage();
        return;
    }
    if(date[DAY] == ERROR || date[DAY] <1){
        ErrorMessage();
        return;
    }

    if(date[YEAR] == ERROR){
        ErrorMessage();
        return;
    }
    if(date[DAY])


    systemClock->day   = date[DAY];
    systemClock->month = date[MONTH];
    systemClock->year  = date[YEAR];

    get_date();
}

//tells the user the time remaining on the alarm
void clear_alarm(){
    setAlarm(VOID_VALUE,VOID_VALUE,VOID_VALUE);
}

/*
 * This function recieves the user arguments for the time in the form xx:xx:xx
 * This function then verifies that the arg's are valid time *
 */
void alarm(char *input_alarm){
    short time[NUM_TIME_ELEMENTS];

    get_time_value(input_alarm, &time);

    if(time_isValid(time[HOUR],HOUR) && time_isValid(time[MINUTE],MINUTE) && time_isValid(time[SECOND], SECOND)){
        setAlarm(time[SECOND],time[MINUTE],time[HOUR]);
    }
    else{
        ErrorMessage();
        return;
    }
}

/*
 * checks the Queue for a time tick
 * returns true and increments time if there
 * returns false if no ticks in queue
 */
short tick(){
    if(dequeue(&clockQueue, FALSE, SYSTICK)){
        //there was a tick in the queue
        systemClock->incrementTime();
        return TRUE;
    }
    else{
        //no tick in queue
        return FALSE;
    }
}


