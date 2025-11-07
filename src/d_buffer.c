/*
 * d_buffer.c
 *
 *  Created on: Nov 07, 2025
 *      Author: Jonathon Walker
 *
 */


#include "d_buffer.h"

/*Returns pointer to empty queue
 * queue will point to node(s) and store size of list
 */
queue_t* allocqueue(void){
	queue_t *temp = malloc(sizeof(queue_t));
	assert(temp != NULL);

	temp->rear = NULL;
	temp->size = 0;
	return temp;
}

/*Adds new node to end of linked lists
 * if linked list is empty queue_t will start initial node; node_t will point to itself
 * if linked list is not empty node is added at rear
 */

void enqueue(queue_t *queue, int id, int data )
{
    assert(queue != NULL);
    node_t *new_nodeptr = malloc(sizeof(node_t));
    assert(new_nodeptr != NULL);

    new_nodeptr->RxData = data;
    new_nodeptr->RxID = id;
    new_nodeptr->next = NULL;

    if (queue->size == 0)
    {
        queue->rear = new_nodeptr;
        queue->rear->next = new_nodeptr;
        queue->size++;
    }
    else
    {
        new_nodeptr->next = queue->rear->next;
        queue->rear->next = new_nodeptr;
        queue->rear = new_nodeptr;
        queue->size++;
    }
}