/*
 * prodcons.c
 *
 *  Created on: Mar 25, 2016
 *      Author: dchiu
 */

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "prodcons.h"


/**
 * Producer threads run this. Produces 10 items.
 */
void* producer(void *args) {
	for (int i = 0; i < 10000; i++) {
		//is buffer full? wait if it is
		sem_wait(buffer_full);

		sem_wait(mutex);	//lock
		//BEGIN critical section:
		//put something in the buffer and update the count
		current_size++;
		buffer[current_size-1] = i;
		printf("Producer %lu put %d. Buffer size = %d\n", pthread_self(), buffer[current_size-1], current_size);
		//END critcal section
		sem_post(mutex);	//unlock

		//let a consumer run
		sem_post(buffer_empty);
	}
	return NULL;
}

/**
 * Consumer threads run this. Consumes 10 items.
 */
void* consumer(void *args) {
	for (int i = 0; i < 10000; i++) {
		//is buffer empty? wait if it is
		sem_wait(buffer_empty);

		sem_wait(mutex);	//lock
		//BEGIN critical section
		printf("Consumer %lu got %d. Buffer size = %d\n", pthread_self(), buffer[current_size-1], current_size);
		current_size--;
		//END critcal section
		sem_post(mutex);	//unlock

		//let a producer run
		sem_post(buffer_full);
	}
	return NULL;
}
