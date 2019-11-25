/*
 * this code contains utility functions and
 * primary functions which process user entries.
 *
 * Utility functions are
 *      AtoI             -> Converts Ascii to Int
 *      compareStrings   -> Boolean check if two strings same
 *
 * Primary functions are
 *       execute_command -> Determines user command and sends to proper handler
 *       process_command -> Gets user entered string and prepares for execute_command
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "systClk.h"
#include "ctype.h"
#include "commands.h"


/*
 * This function takes a pointer to a string and the length of the string
 * This function then returns the int value of the number in the string
 * if there isn't a number, returns ERROR. All values in this code > 0, ERROR = -1
 * to handle leading whitespace, do nothing
 *
 * if the string is null terminated, return val
 */
short AtoI(char *integer, const short str_length){
    short val = 0;
    short length = 0;

    while(*integer && length<str_length){
        if(isdigit(*integer)){
            val = val*10 + (*integer-'0');
            length++;
        }
        else if(*integer == WHITE_SPACE){
            //do nothing
            val = val;
        }
        else{
            return ERROR;
        }
        integer++;
    }
    return val;
}

/*
 * returns true if strings first 'length' characters same
 * case insensitive
 */
unsigned char compareStrings(char *source, char *dest, short length){
    unsigned char i;
    for(i=0; i<length;i++){
        if(toupper(*source) != toupper(*dest)){
            return FALSE;
        }
        source++;
        dest++;
    }
    //only gets here if true
    return TRUE;
}

/*
 * A pointer to the string that the user entered as a command
 * The command refers to time, alarm, or date
 * the args contains the arguments passed
 * if no args, index == 0
 * no args, execute get_time, get_date, clear_alarm
 * if args, execute set_time, set_date, set_alarm
 * if the entry is invalid in any way, ERROR
 * do nothing, warn user
 */
void execute_command(char* iterator){

    //tokenize the command
    unsigned char index = 0;

    char command[COMMAND_SIZE];
    char args[ARGUMENT_SIZE];

//    this isolates the user entered command
    while((*iterator != SPACE_KEY && *iterator != NULL)&& index<COMMAND_SIZE ){
        command[index] = tolower(*iterator);
        index++;
        iterator++;
    }

    //reset the array index
    index = 0;
    //there is a space between the args and command
    if(*iterator == SPACE_KEY)
        iterator++;

    //This isolates the arguments to the command
    while(*iterator != NULL){
        args[index] = *iterator;
        index++;
        iterator++;
    }
    args[index] = NULL;
    if(compareStrings(command,"time ",COMMAND_SIZE))
        if(index == 0)
            get_time();
        else
            set_time(args);
    else if(compareStrings(command,"date ",COMMAND_SIZE))
        if(index == 0)
            get_date();
        else
            set_date(args);
    else if(compareStrings(command,"alarm",COMMAND_SIZE))
        if(index == 0)
            clear_alarm();
        else
            alarm(args);
    else
        writeStringToConsole("Bad Entry");
}

/*
 * The input Queue is processed.
 * All data from lag to lead collected in a string
 * execute function execute_command which handles deciphering user entry
 */
void process_command(QUEUE *p){

    char entry[COMMAND_SIZE+ARGUMENT_SIZE];
    unsigned char index = 0;

    //Populate the command string
    while (p->lagIndex != p->leadIndex){
        entry[index] = p->buffer[p->lagIndex];
        index++;
//        p->lagIndex++;
        incrementIndex(&p->lagIndex);
    }

    //set a NULL marker which indicates the end of the user entry
    entry[index]= NULL;

    //pass a pointer to the first index of command
    execute_command(entry);
}
