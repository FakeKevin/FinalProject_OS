/*
OS Term Project
Date Due: Nov. 19th, 2018
By: Jason Han, Kevin Jee, Daryl Saldanha

Description:

Producer–Consumer Problem using Pthreads (also known as POSIX
threads)

Section 5.7.1 presented a semaphore-based solution to the producer–consumer
problem using a bounded buffer. In this project, you will design a programming solution
to the bounded-buffer problem using the producer and consumer processes shown in
Figures 5.9 and 5.10. The solution presented in Section 5.7.1 uses three semaphores:
empty and full, which count the number of empty and full slots in the buffer, and
mutex, which is a binary (or mutual-exclusion) semaphore that protects the actual
insertion or removal of items in the buffer. For this project, you will use standard
counting semaphores for empty and full and a mutex lock, rather than a binary
semaphore, to represent mutex. The producer and consumer—running as separate
threads—will move items to and from a buffer that is synchronized with the empty,
full, and mutex structures. Solve this problem using Pthreads, using unnamed
semaphores.

Unnamed vs. Named Semaphores

POSIX semaphores can be unnamed or named. Unnamed semaphores are allocated in
process memory and initialized. Unnamed semaphores might be usable by more than
one process, depending on how the semaphore is allocated and initialized. Unnamed
semaphores are either private, inherited through fork(), or are protected by access
protections of the regular file in which they are allocated and mapped.
Named semaphores are like process-shared semaphores, except that named
semaphores are referenced with a pathname rather than a pshared value. Named
semaphores are sharable by several processes. Named semaphores have an owner
user-id, group-id, and a protection mode.
The functions sem_open, sem_getvalue, sem_close, and sem_unlink are available
to open, retrieve, close, and remove named semaphores. By using sem_open,
you can create a named semaphore that has a name defined in the file system name
space.

References:

https://www.geeksforgeeks.org/multithreading-c-2/
https://stackoverflow.com/questions/16320354/passing-pointer-char-argument-to-function-in-thread
https://stackoverflow.com/questions/1352749/multiple-arguments-to-function-called-by-pthread-create
https://stackoverflow.com/questions/36050901/how-to-pass-command-line-arguments-to-thread-function
https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
https://www.geeksforgeeks.org/use-posix-semaphores-c/

*/
#include <stdio.h>
#include <stdlib.h> /* required for rand() */
#include "buffer.h"
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;
int buffer[5] = {-1,-1,-1,-1,-1};

int insert_item(int item) {
/* insert item into buffer
return 0 if successful, otherwise
return -1 indicating an error condition */
	int toReturn = -1; 
	int n = 0;
	
	while(n < 5 && toReturn == -1)
	{
		if(buffer[n] == -1)
		{
			toReturn = n;
		}
		n++;
	}
	
	if(toReturn != -1)
	{
		buffer[toReturn] = item;
		toReturn = 0;
	}
	return toReturn;
}

int remove_item() {
/* remove an object from buffer
placing it in item
return 0 if successful, otherwise
return -1 indicating an error condition */
	int toReturn = -1; 
	int n = 0;
	
	while(n < 5 && toReturn == -1)
	{
		if(buffer[n] != -1)
		{
			toReturn = n;
		}
		n++;
	}
	if(toReturn != -1)
	{
		int temp = toReturn;
		toReturn = buffer[toReturn];
		buffer[temp] = -1;
	}
	return toReturn;
}

// ALL Producer related code:------------------------------------------------
void *producer(void *param) {
	int item;
	int b = 1;
	while (b == 1) {
		printf("Created a producer\n");
		sem_wait(&mutex);
		/* sleep for a random period of time */
		srand(time(NULL));
		//Sleep for some number between 0 & 500
		int r = rand() % 10; 
		sleep(r);
		/* generate a random number */
		//To keep the IDs low, only generate between 100,000
		item = rand() % 100000;
		if (insert_item(item) == -1)
			fprintf(stderr,"Could not insert item into buffer\n");
		else
			printf("producer produced %d\n",item);
		sem_post(&mutex);
		b = 0;
	}
}


// ALL Consumer related code:------------------------------------------------
void *consumer(void *param) {
	int item;
	int b = 1;
	while (b == 1) {
		printf("Created a consumer\n");
		sem_wait(&mutex);
		/* sleep for a random period of time */
		srand(time(NULL));
		//Sleep for some number between 0 & 500
		int r = rand() % 10; 
		sleep(r);
		if ((item = remove_item()) == -1)
			fprintf(stderr, "Could not remove item from buffer\n");
		else	
			printf("consumer consumed %d\n",item);
		sem_post(&mutex);
		b = 0;
	}
}

// Main code: ----------------------------------------------------------
int main(int argc, char *argv[]) {
	
	if(argc < 4)
	{
		fprintf(stderr,"Incorrect number of arguments. Should give ./prog n1 n2 n3\n");
		exit(1);
	}
	/* 1. Get command line arguments
	argv[1],argv[2],argv[3] */
	int numProducer = atoi(argv[2]);
	int numConsumer = atoi(argv[3]);
	/* 2. Initialize buffer */	
	//Buffer is declared already as a global variable
	sem_init(&mutex, 0 , 1);
	/* 3. Create producer thread(s) */
	int n = 1;
	for(n = 1; n < numProducer+1; n++)
	{
		pthread_t tid = n*10;
		pthread_create(&tid, NULL, producer, NULL);
		pthread_join(tid, NULL);
	}

	/* 4. Create consumer thread(s) */
	n = 1;
	for(n = 1; n < numConsumer+1; n++)
	{
		pthread_t tid2 = n*11;
		pthread_create(&tid2, NULL, consumer, NULL);
		pthread_join(tid2, NULL);
	}
	
	/* 5. Sleep */

	sleep(atoi(argv[1]));
	
	/* 6. Exit */
	sem_destroy(&mutex);
	
	printf("\nCompleted.\n");
	return 0;
}


