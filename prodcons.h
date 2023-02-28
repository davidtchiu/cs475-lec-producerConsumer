/*
 * producer.h
 *
 *  Created on: Mar 25, 2016
 *      Author: dchiu
 */

#ifndef PRODCONS_H_
#define PRODCONS_H_


//these external variables were declared in main.c
extern sem_t *buffer_full, *buffer_empty,  *mutex;
extern int *buffer;
extern int current_size;

//prototypes
void* producer(void *);
void* consumer(void *);

#endif /* PRODCONS_H_ */