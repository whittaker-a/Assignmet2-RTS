/*
 * The software that controls the UART ISR
 *
 * Provided and tweaked to add
 * Queues,
 * Error Messages,
 * And Output Functionality
 *
 */


/* Globals */
volatile char Data;     /* Input data from UART receive */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UART0.h"
#include "commands.h"
#include "queueStruct.h"
#include "Process.h"
QUEUE UART0_Queue = {};

extern PCB *running;

static line_cursor output_line = {ESC, '[', '0', '0', ';', '0', '0', 'H', NUL};

/*
 * This function queue's a string to the output buffer
 * Then Enqueue's a new line character and
 * forces the first character
 * The rest happens during the UART interrupt
 */
void writeCharToConsole(char data){
    /*  Checks if UART FIFO Full    */
    while(UART0_FR_R & UART_FR_TXFF);
    UART0_DR_R = data;
}

void writeStringToConsole(char *index){
    while(*index)
        writeCharToConsole(*(index++));
}

//void print(char *index){
//
//    sprintf(output_line.line, "%02i", running->PID);
//    output_line.semicolon = ';';
//    writeStringToConsole((char *)&output_line);
//    writeStringToConsole(index);
//
//}

void writeStringToLine(unsigned int line, char * out){

     sprintf(output_line.line, "%i", line);
     output_line.semicolon = ';';
     output_line.col[0] = 0;
     output_line.col[1] = 0;
     output_line.cmdchar = 'H';
     writeStringToConsole((char *)&line);
     writeStringToConsole(out);

}


void ErrorMessage(){
    writeStringToConsole("Error - Bad Command");
}

void proccessUserEntry(volatile const char *data){
    //process the one key and back out of the program
    if(*data == ENTER_KEY){
        //User entered a command
        process_command(&UART0_Queue);
    }
    else if(*data == BACKSPACE){
        if(UART0_Queue.leadIndex!=UART0_Queue.lagIndex){
            decrementIndex(&UART0_Queue.leadIndex);
        }
    }
    else{
        enqueue(&UART0_Queue, data, UART);
    }
}

void UART0_Init(void)
{
    volatile int wait;

    /* Initialize UART0 */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCUART_GPIOA;   // Enable Clock Gating for UART0
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCGPIO_UART0;   // Enable Clock Gating for PORTA
    wait = 0; // give time for the clocks to activate

    UART0_CTL_R &= ~UART_CTL_UARTEN;        // Disable the UART
    wait = 0;   // wait required before accessing the UART config regs

    // Setup the BAUD rate
    UART0_IBRD_R = 8;   // IBRD = int(16,000,000 / (16 * 115,200)) = 8.680555555555556
    UART0_FBRD_R = 44;  // FBRD = int(.680555555555556 * 64 + 0.5) = 44.05555555555556

    UART0_LCRH_R = (UART_LCRH_WLEN_8);  // WLEN: 8, no parity, one stop bit, without FIFOs)

    GPIO_PORTA_AFSEL_R = 0x3;        // Enable Receive and Transmit on PA1-0
    GPIO_PORTA_PCTL_R = (0x01) | ((0x01) << 4);         // Enable UART RX/TX pins on PA1-0
    GPIO_PORTA_DEN_R = EN_DIG_PA0 | EN_DIG_PA1;        // Enable Digital I/O on PA1-0

    UART0_CTL_R = UART_CTL_UARTEN;        // Enable the UART
    wait = 0; // wait; give UART time to enable itself.
}

void InterruptEnable(unsigned long InterruptIndex)
{
/* Indicate to CPU which device is to interrupt */
if(InterruptIndex < 32)
    NVIC_EN0_R = 1 << InterruptIndex;       // Enable the interrupt in the EN0 Register
else
    NVIC_EN1_R = 1 << (InterruptIndex - 32);    // Enable the interrupt in the EN1 Register
}

void UART0_IntEnable(unsigned long flags)
{
    /* Set specified bits for interrupt */
    UART0_IM_R |= flags;
}

void InterruptMasterDisable(){
    /* disable CPU interrupts */
    __asm(" cpsid   i");
}

void InterruptMasterEnable(void)
{
    /* enable CPU interrupts */
    __asm(" cpsie   i");
}

void UART0_IntHandler(void)
{
/*
 * Simplified UART ISR - handles receive and xmit interrupts
 * Application signaled when data received
 */
    if (UART0_MIS_R & UART_INT_RX)
    {
        /* RECV done - clear interrupt and make char available to application */
        UART0_ICR_R |= UART_INT_RX;
        Data = UART0_DR_R;
//        proccessUserEntry(&Data);
    }

    if (UART0_MIS_R & UART_INT_TX)
    {
        /* XMIT done - clear interrupt */
        //need to add a dump queue routine
        UART0_ICR_R |= UART_INT_TX;
//        char temp;
//        while(dequeue(&UART0_Out,&temp,UART)){
//            UART0_DR_R = temp;
//            while(!(UART0_MIS_R & UART_INT_TX));
//        }
    }
}



