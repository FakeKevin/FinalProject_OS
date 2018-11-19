/*
OS Term Project
Date Due: Nov. 19th, 2018
By: Jason Han, Keun Young Jee, Daryl Saldanha
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
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

#define TRUE 1

/* Variables */
sem_t full, empty; 	  	 //Semaphores
pthread_mutex_t mutex;    	 //Mutex Lock
buffer_item buffer[BUFFER_SIZE]; //Buffer
int counter;		  	 //Buffer Counter
pthread_t tid;		  	 //Thread ID
pthread_attr_t attr;	  	 //Set of thread attributes

void *producer(void *param); //Producer thread
void *consumer(void *param); //Consumer thread

void run(){
  /* Initialize the mutex lock, full and empty semaphores, attributes and buffer */
  pthread_mutex_init(&mutex, NULL);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, BUFFER_SIZE);
  pthread_attr_init(&attr);
  counter = 0;
}

int insert_item(buffer_item item) {
  /* Insert item into buffer return 0 if successful, otherwise
  return -1 indicating an error condition */
  if(counter < BUFFER_SIZE){
    buffer[counter] = item;
    counter++;
    return 0;
  }
  else {
    return -1;
  }
}

int remove_item(buffer_item *item) {
  /* remove an object from buffer placing it in item return 0 if successful, 
  otherwise return -1 indicating an error condition */
  if(counter > 0){
    *item = buffer[(counter - 1)];
    counter--;
    return 0;
  }
  else {
    return -1;
  }
}

// ALL Producer related code:------------------------------------------------
void *producer(void *param) {
  buffer_item item;

  while (TRUE) {
    /* Sleep for some random number between 0 & 500 */
    int r = rand() % 10;
    sleep(r);

    /* Generate a random number */
    item = rand();

    /* Acquire the empty lock */
    sem_wait(&empty);
    /* Acquire the mutex lock */
    pthread_mutex_lock(&mutex);

    if (insert_item(item)) {
       fprintf(stderr,"Could not insert item into buffer\n");
    }
    else {
       printf("Producer produced %d\n",item);
    }

    /* Release mutex lock */
    pthread_mutex_unlock(&mutex);
    /* Signal full */
    sem_post(&full);
  }
}

// ALL Consumer related code:------------------------------------------------
void *consumer(void *param) {
  buffer_item item;

  while (TRUE) {
    /* Sleep for some random number between 0 & 500 */
    int r = rand() % 10;
    sleep(r);

    /* Acquire full lock */
    sem_wait(&full);
    /* Aquire the mutex lock */
    pthread_mutex_lock(&mutex);

    if (remove_item(&item)){
       fprintf(stderr, "Could not remove item from buffer\n");
    }
    else {
       printf("Consumer consumed %d\n",item);
    }

    /* Release mutex lock */
    pthread_mutex_unlock(&mutex);
    /* Signal empty */
    sem_post(&empty);
  }
}

// Main code: ----------------------------------------------------------
int main(int argc, char *argv[]) {

 /* Verifying the arguments that are being passed */
 if(argc != 4) {
    fprintf(stderr,"Incorrect number of arguments. Should give ./prog n1 n2 n3\n");
    exit(1);
 }

 /* 1. Get command line arguments argv[1],argv[2],argv[3] */
   int waitTime = atoi(argv[1]);    // Wait time in seconds
   int numProducer = atoi(argv[2]); // Number of producer threads
   int numConsumer = atoi(argv[3]); // Number of consumer threads

 /* 2. Initialize buffer */
   run();

 /* 3. Create producer thread(s) */
   int n;
   for(n = 0; n < numProducer; n++) {
     pthread_create(&tid, &attr, producer, NULL);
   }

 /* 4. Create consumer thread(s) */
   for(n = 0; n < numConsumer; n++) {
     pthread_create(&tid, &attr, consumer, NULL);
   }

 /* 5. Sleep */
   sleep(waitTime);

 /* 6. Exit */
   sem_destroy(&full);
   sem_destroy(&empty);

   printf("\nCompleted.\n");
   exit(1);
}
