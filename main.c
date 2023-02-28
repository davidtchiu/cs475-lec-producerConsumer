#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include "prodcons.h"

#define NUM_THREADS 2
#define BUFFER_SIZE 10

void init_semaphores();

//declare semaphores in shared space
sem_t *buffer_full;
sem_t *buffer_empty;
sem_t *mutex;

//shared buffer and item count
int current_size;
int *buffer;

int main(int argc, char *argv[]) {
	//initialize semaphores
	init_semaphores();

	//initialize shared data
	current_size = 0;
	buffer = (int*) malloc(sizeof(int) * BUFFER_SIZE);

	//create consumer and producer threads
	pthread_t *prod = (pthread_t*) malloc(NUM_THREADS * sizeof(pthread_t));
	pthread_t *cons = (pthread_t*) malloc(NUM_THREADS * sizeof(pthread_t));

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&prod[i], NULL, producer, NULL);
		pthread_create(&cons[i], NULL, consumer, NULL);
	}

	//join all threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(prod[i], NULL);
		pthread_join(cons[i], NULL);
	}

	//close semaphores
	sem_close(buffer_full);
	sem_close(buffer_empty);
	sem_close(mutex);

	//delete semaphores
	sem_unlink("/FullBuffer");
	sem_unlink("/EmptyBuffer");
	sem_unlink("/Lock");

	//free up memory
	free(prod);
  prod = NULL;

	free(cons);
  cons = NULL;

	return 0;
}

/**
 * Initialize semaphores
 */
void init_semaphores() {
	//remove semaphores if they were never deleted
	sem_unlink("/FullBuffer");
	sem_unlink("/EmptyBuffer");
	sem_unlink("/Lock");

	//create them again
	buffer_full = sem_open("/FullBuffer", O_CREAT, 0600, BUFFER_SIZE);
	buffer_empty = sem_open("/EmptyBuffer", O_CREAT, 0600, 0);
	mutex = sem_open("/Lock", O_CREAT, 0600, 1); //just a lock
}



