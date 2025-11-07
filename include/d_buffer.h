/*
 * d_buffer.h
 *
 *  Created on: Nov 5, 2025
 *      Author: User
 */

#ifndef INC_D_BUFFER_H_
#define INC_D_BUFFER_H_
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "stm32f1xx_hal.h"


// Buffer as circular linked-list has one instance of queue_t which points to
// end node(s) of linked list.

typedef struct node{
	struct node *next;
	uint32_t RxData;
	uint8_t RxID;
	//...add more later
}node_t;

typedef struct{
	node_t *rear;
	uint8_t size;
}queue_t;

queue_t* allocqueue(void);
void enqueue(queue_t *queue, int id, int data );

#endif /* INC_D_BUFFER_H_ */
