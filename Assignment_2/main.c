/**
 *
 * This Software allows for multiple processes, at given priority levels,
 * to operate pseudo-simultaneously.
 * Each process must be of type void with no return value.
 * These processes can:
 *      Request their process ID
 *      Change priority Levels
 *      terminate
 *
 * and this software supports inter-process communication where
 * each process can request a mailbox from the kernel, once
 * a mailbox is allocated to the process, the process can
 *      Send Messages
 *      Receive Messages
 *      relieve control over their mailbox
 *
 * Author: Alexander Whittaker
 * Fall 2019
 */

#include <stdio.h>
#include <stdlib.h>

#include "UART0.h"
#include "sysTick.h"

//#include "queueStruct.h"
//#include "commands.h"
//#include "systClk.h"

#include "Process.h"
#include "registerProcess.h"
#include "testProcess.h"
#include "priorityQueue.h"
#include "ServiceCalls.h"

//Initializes the UART interrupts and SysTick
void main (void){
    /* Initialize UART */

    //initializes each priority Queue
    initPriorityQueue();
    initMailBoxList();

//    func_name testVt = &vt_100_proc;
//    reg_proc(testVt, 375, 3);
//    func_name testVT2 = &blink;
//    reg_proc(testVT2, 5, 3);

//    func_name testPID = &writePID;
//    reg_proc(testPID, 3, 3);

    func_name communication = &communications;
    reg_proc(communication, 1, 4);

    func_name proc = &testProcWrite;
    reg_proc(proc, 12, 2);


//    func_name testproc2 = &writeYtoConsoleAndDie;
//    reg_proc(testproc2, 700, 3);



    UART0_Init();                               // Initialize UART0
    InterruptEnable(INT_VEC_UART0);              // Enable UART0 interrupts
    UART0_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts

//    init_AlarmClock();
//    init_SystemClock();

    /* Sets the period at which the System Clock Increments*/
    SysTickPeriod(MAX_WAIT);

    //Get the first process to run
//    setRunning();

    //start the first function
    init_PendSVPriority();


    SVC();

    /*  Never Reachers This point   */

//    while(TRUE);



}






