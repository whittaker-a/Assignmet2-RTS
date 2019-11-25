/*
 *  This software handle time interrupts
 *  Used to accurately manage the system clock
 *
 *
 */

#include "sysTick.h"
//#include "systClk.h"

#include "priorityQueue.h"
#include "ServiceCalls.h"

void SysTickStart(void)
{
    // Set the clock source to internal and enable the counter to interrupt
    ST_CTRL_R |= ST_CTRL_CLK_SRC | ST_CTRL_ENABLE;
}

void SysTickStop(void)
{
    // Clear the enable bit to stop the counter
    ST_CTRL_R &= ~(ST_CTRL_ENABLE);
}

void SysTickPeriod(unsigned long Period)
{
    /*
     For an interrupt, must be between 2 and 16777216 (0x100.0000 or 2^24)
    */
    ST_RELOAD_R = Period;
}

void SysTickIntEnable(void)
{
    // Set the interrupt bit in STCTRL
    ST_CTRL_R |= ST_CTRL_INTEN;
}

void SysTickIntDisable(void)
{
    // Clear the interrupt bit in STCTRL
    ST_CTRL_R &= ~(ST_CTRL_INTEN);
}

// global variable to count number of interrupts on PORTF0 (falling edge)
volatile int count = 0;

void IntMasterEnable(void)
{
    // enable CPU interrupts
    __asm(" cpsie   i");
}

void SysTick_init(){
    SysTickIntEnable();
    SysTickStart();
    void IntMasterEnable(void);
}

void SysTickHandler(void)
{
//    nextProcess();
    PendSV_Handler();
}
