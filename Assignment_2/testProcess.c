/*
 * testProcess.c
 *
 *
 * These functions are the process which run synchronously
 * they are used for testing the other software
 * Each process is a C function of type void
 *
 *
 *  Created on: Nov 4, 2019
 *      Author: AWhit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UART0.h"
#include "Kernel_Calls.h"
#include "ServiceCalls.h"
#include "testProcess.h"

static LINE command_line = {ESC, '[', 1, ';', 0, 'H', NULL};


//this function writes the char 'X' to the console
void writeXtoConsole(){
    char temp = 'X';
    while(1){
        writeStringToConsole(&temp);
    }
}

void infiniteLoop(){
    int i = 0;

    while(100){
        i++;
        i--;
    }
}

void writeYtoConsoleAndDie(){
    int i=0;
    char temp = 'Y';
    while(i<100){
        i++;
        writeStringToConsole(&temp);
    }
}

void writeZtoConsole(){
    char temp = 'Z';
    while(1){
        writeStringToConsole(&temp);
    }
}


void prioritySwitchTo2(){
    int i = 0;
    while(i<1000){
        i++;
    }
    nice(2);

    //just stay here
    while(1);
}


void cascadeNice(){
    nice(4);
    nice(3);
    nice(2);
    nice(1);
    while(1);
}

void testBind(){

    bind(2);
    char temp = 'B';
    while (1){
        writeStringToConsole(&temp);
    }

}

int has_sent = 0;

void receiveAndWrite(){

    bind(3);
    char *msg = malloc(12 * sizeof(char));

//    unsigned int i = 0;

//    while(has_sent == 0);

    while(1){
        recv(3, 1, msg, 12);
        writeStringToConsole(msg);
    }
}

void sendAndWait(){

    bind(1);
    char* msg = malloc(6 * sizeof(char));
    strcpy(msg, "HELLO");

    short sent = 0;

    while(sent <=0){
        sent = send(3, 1, msg, 6);
    }
//    has_sent = 1;

    nice(1);
    while(1);
}




void blink(){
    int i = 0;
    while(1){
        while(i<100000){
            i++;
        }
        i=0;
        writeStringToLine(5 ,"A");
        while(i<100000){
            i++;
        }
        i=0;
        writeStringToLine(5 ,"B");
    }
}



void vt_100_proc(){

    struct CUP cup = {ESC, '[', '0', '0', ';', '0', '0', 'H', NUL};

    struct CUP g1 = {ESC, ')', NULL};
//    g1.esc = ESC;
//    g1.sqrbrkt = '(';
    writeStringToConsole((char *)&g1);

    cup.line[0]=0;
    cup.line[1]=4;
    cup.col[0] = 3;
    cup.col[1] = 3;
    cup.cmdchar = 'm';



    writeStringToConsole((char*)&cup);

    cup . line [0] = '1';
    cup . line [1] = '9';
    cup . col [0] = '0';
    cup . col [1] = '5';
    cup.cmdchar = 'H';
    /*
     - To test on screen - change ESC to 'E'
     - If removed when using a VT-100 emulator, cursor will be reposition
    */
//    cup . esc = 'E';

//    printf("\u001b[31mHelloWorld");
//    printf("%s\n", (char *)&cup);
    writeStringToConsole((char*)&cup);
    writeStringToConsole("Hello");

}
void testProcWrite(){
    bind(5);
    char * msg = "Great";
    send(1, 5, msg, 5);
}

void writePID(){
    unsigned long pid;
    pid = get_pid();

    char * out = malloc(sizeof(char)*2);

    sprintf(out, "%i", pid);
    writeStringToConsole(out);
}

/*
 * Receives from any, writes contents to screen
 */
void communications(){
    bind(1);

    char * out = malloc(sizeof(char) * MAX_LETTER_SIZE);

    while(1){
        if(recv(ANY, ANY, out, MAX_LETTER_SIZE ) > 0){
            writeStringToConsole("\n");
            writeStringToConsole(out);
        }
    }
}

