/*
 * The Header file for QueueStruct.
 * Here the Size of Queue is defined and
 * The struct QUEUE is defined
 * Used for input Queue, output queue, sysTick queue
 *
 *  Created on: Sep 22, 2019
 *      Author: AWhit
 */

#ifndef QUEUESTRUCT_H_
#define QUEUESTRUCT_H_


#define QUEUE_SIZE 32
#define START_QUEUE 0

#define TRUE 1
#define FALSE 0

enum INTR {UART,SYSTICK};

typedef struct queue_struct{

    char  buffer[QUEUE_SIZE];
    short leadIndex;
    short lagIndex;

}QUEUE;



void incrementIndex(short *index);
void decrementIndex(short *index);
short dequeue(QUEUE *self, char * data,  enum INTR int_type);
short enqueue(QUEUE *self, const volatile char * data, enum INTR int_type);

#endif /* QUEUESTRUCT_H_ */
