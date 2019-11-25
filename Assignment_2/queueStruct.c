/*
 *
 *  The Purpose of this code is to implement a structure which is responsible for both storing
 *  and managing the UART0 Data buffer.
 *
 *  This contains the 32 char buffer array
 *  The lead and lag index
 *
 *
 */
#include <stdio.h>

#include "queueStruct.h"
#include "UART0.h"
#include "systClk.h"



/*
 * Increments the index of the queue. if index value overflows circular buffer value
 * then reset position to 0
 */
void incrementIndex(short *index){
    *index = *index + 1;
    if(*index >= QUEUE_SIZE){
        *index=0;
    }
}

/*
 * Decrements Index by 1. If value goes below 0, index
 * wraps around to circular buffer max value
 */
void decrementIndex(short *index){
    *index = *index - 1;
    if(*index < 0){
        //max buffer index is size - 1
        *index = QUEUE_SIZE-1;
    }
}

//this is this function checks the user input character and acts as required
//backspace -> backspace
//Enter -> initiate command
//Calculates the number of items in the Queue
//either lead ahead of lag and the difference is queuesize - 1
short queueSize(QUEUE *p){
//    short entries;
    if (p->leadIndex >= p->lagIndex){
       return p->leadIndex - p->lagIndex;
    }
    else
        return p->leadIndex +(QUEUE_SIZE - p->lagIndex-1);
}

/*
 * Lead index always points to the index previous to where the lag index points
 * if full, return TRUE
 * else,    return FALSE
 */
short is_queueFull(QUEUE *self){
    //queue full when lead index points to the space behind
    if(queueSize(self) == QUEUE_SIZE - 1){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

/*
 * if the lag and lead point to the same place in memory, the queue is empty
 * the lead always ends one place behind the lag
 */
short queueEmpty(QUEUE *self){
    if(self->leadIndex == self->lagIndex)
        return TRUE;
    else
        return FALSE;
}


/*
 * pass a char as an argument, dequeues from
 * a queue, which is also passed. Takes the type of queue (UART, SYSTICK)
 * If systick tick being removed, increment time, ignore char
 * if uart being dequeued, set data to the removed value
 */
short dequeue(QUEUE *self, char * data,  enum INTR int_type){
    //kicks out if queue is empty
    if(queueEmpty(self)){
        return FALSE;
    }

    //check interrupt type
    switch(int_type){
    case UART :
        //add data to queue
        *data = self->buffer[self->lagIndex];
        break;
    case SYSTICK :
        //process a tick, no data removed from queue
        break;
    }
    //point to the next item in the queue
    incrementIndex(&self->lagIndex);
    return TRUE;
}

/*
 * Takes in a Queue, a char, and an interrupt type
 * if systick, add a tick to the queue
 * if uart, add the char to front of queue
 * always set the lead to point to a null
 * only enqueue when Queue not full
 */
short enqueue(QUEUE *self, const volatile char * data, enum INTR int_type){
    //kicks out if queue empty
    if(is_queueFull(self)){
            return FALSE;
        }

    switch(int_type){
    case UART :
        //adds data to buffer at the location of leadIndex and increments.
        self->buffer[self->leadIndex] = *data;
        break;
    case SYSTICK :
        //is systick, add a tick
        self->buffer[self->leadIndex] = TRUE;
        break;
    }
    //always point to a null
    incrementIndex(&self->leadIndex);
    self->buffer[self->leadIndex] = NULL;

    //data was added to the queue
    return TRUE;
}

