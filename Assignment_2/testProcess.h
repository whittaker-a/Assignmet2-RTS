/*
 * testProcess.h
 *
 *  Created on: Nov 4, 2019
 *      Author: AWhit
 */

#ifndef TESTPROCESS_H_
#define TESTPROCESS_H_

void writeXtoConsole();
void writeYtoConsoleAndDie();
void prioritySwitchTo2();
void writeZtoConsole();

void cascadeNice();
void testBind();
void sendAndWait();
void receiveAndWrite();

void vt_100_proc();
void blink();

void testProcWrite();
void communications();
void testWriteto3();
void testWriteto2();

void writePID();


typedef struct cursor
{
    char esc;
    char sqrbrkt;
    short line;   /* 01 through 24 */
    char semicolon;
    short col;    /* 01 through 80 */
    char cmdchar;
    char nul;

}LINE;


#endif /* TESTPROCESS_H_ */
