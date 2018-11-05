# OS_Project

##Producer–Consumer Problem using Pthreads (also known as POSIX threads)

Section 5.7.1 presented a semaphore-based solution to the 
producer–consumer
problem using a bounded buffer. In this project, you will design a 
programming solution
to the bounded-buffer problem using the producer and consumer processes 
shown in
Figures 5.9 and 5.10. The solution presented in Section 5.7.1 uses three 
semaphores:
empty and full, which count the number of empty and full slots in the 
buffer, and
mutex, which is a binary (or mutual-exclusion) semaphore that protects 
the actual
insertion or removal of items in the buffer. For this project, you will 
use standard
counting semaphores for empty and full and a mutex lock, rather than a 
binary
semaphore, to represent mutex. The producer and consumer—running as 
separate
threads—will move items to and from a buffer that is synchronized with 
the empty,
full, and mutex structures. Solve this problem using Pthreads, using 
unnamed
semaphores.

##Unnamed vs. Named Semaphores
POSIX semaphores can be unnamed or named. Unnamed semaphores are 
allocated in
process memory and initialized. Unnamed semaphores might be usable by 
more than
one process, depending on how the semaphore is allocated and 
initialized. Unnamed
semaphores are either private, inherited through fork(), or are 
protected by access
protections of the regular file in which they are allocated and mapped.
Named semaphores are like process-shared semaphores, except that named
semaphores are referenced with a pathname rather than a pshared value. 
Named
semaphores are sharable by several processes. Named semaphores have an 
owner
user-id, group-id, and a protection mode.
The functions sem_open, sem_getvalue, sem_close, and sem_unlink are 
available
to open, retrieve, close, and remove named semaphores. By using 
sem_open,
you can create a named semaphore that has a name defined in the file 
system name
space.

##The Buffer
Internally, the buffer will consist of a fixed-size array of type 
buffer_item (which will
be defined using a typedef). The array of buffer_item objects will be 
manipulated
as a circular queue. The definition of buffer_item, along with the size 
of the buffer,
can be stored in a header file such as the following:
/* buffer.h */
typedef int buffer item;
#define BUFFER SIZE 5
The buffer will be manipulated with two functions, insert_item() and
remove_item(), which are called by the producer and consumer threads, 
respectively.
Below is a skeleton outlining these functions.
Outline of Buffer Operations:

```
#include "buffer.h"
/* the buffer */
buffer item buffer[BUFFER SIZE];
int insert item(buffer item item) {
/* insert item into buffer
return 0 if successful, otherwise
return -1 indicating an error condition */
}
int remove item(buffer item *item) {
/* remove an object from buffer
placing it in item
return 0 if successful, otherwise
return -1 indicating an error condition */
}
```

The insert_item() and remove_item()functions will synchronize the 
producer and
consumer using the algorithms outlined in Figures 5.9 and 5.10. The 
buffer will also
require an initialization function that initializes the mutual-exclusion 
object mutex along
with the empty and full semaphores.

The main() function will initialize the buffer and create the separate 
producer and
consumer threads. Once it has created the producer and consumer threads, 
the main()
function will sleep for a period of time and, upon awakening, will 
terminate the
application. The main() function will be passed three parameters on the 
command
line:
1. How long to sleep before terminating
2. The number of producer threads
3. The number of consumer threads
Below is a skeleton for this function.
Outline of Skeleton Program

```
#include "buffer.h"
int main(int argc, char *argv[]) {
/* 1. Get command line arguments
argv[1],argv[2],argv[3] */
/* 2. Initialize buffer */
/* 3. Create producer thread(s) */
/* 4. Create consumer thread(s) */
/* 5. Sleep */
/* 6. Exit */
}
```

##The Producer and Consumer Threads
The producer thread will alternate between sleeping for a random period 
of time and
inserting a random integer into the buffer. Random numbers will be 
produced using the
rand() function, which produces random integers between 0 and RAND_MAX. 
The
consumer will also sleep for a random period of time and, upon 
awakening, will attempt
to remove an item from the buffer. Below is an outline of the producer 
and consumer
threads.
Outline of the Producer and Consumer Threads

```
#include <stdlib.h> /* required for rand() */
#include "buffer.h"
void *producer(void *param) {
buffer item item;
while (true) {
/* sleep for a random period of time */
sleep(...);
/* generate a random number */
item = rand();
if (insert item(item))
fprintf("report error condition");
else
printf("producer produced %d\n",item);
}
}
void *consumer(void *param) {
buffer item item;
while (true) {
/* sleep for a random period of time */
sleep(...);
if (remove item(&item))
fprintf("report error condition");
else
printf("consumer consumed %d\n",item);
}
}
```

##Pthreads Thread Creation and Synchronization
Creating threads using the Pthreads API is discussed in Section 4.4.1. 
Coverage of
mutex locks and semaphores using Pthreads is provided in Section 5.9.4. 
Refer to those
sections for specific instructions on Pthreads thread creation and 
synchronization.
