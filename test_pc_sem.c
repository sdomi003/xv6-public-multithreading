#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

struct MultiArgs
{
   int sum;
   int *sem;
   int *sem_prod; // produce and consume semaphores
   int *sem_cons;
};

/*
 * PRODUCER CONSUMER PROBLEM
 * Producer Thread adds to an integer value.
 * Consumer Thread prints that value.
 * Without a semaphore, consumer thread will increase the integer value by
 * 	more than one before the consumer gets to print it.
 * So, a working program will print out "123456..." without skipping any numbers and without diplicating numbers.
 * Using semaphores and threads.
*/

void producer(void *arg){
	struct MultiArgs *ma = (struct MultiArgs*)arg;
    int i;
	for (i = 0; i < 100; ++i) {
		sem_wait(ma->sem_prod);
		ma->sum += 1;
		sem_post(ma->sem_cons);	
	}	
	exit();

}
void consumer(void *arg) {
	//printf(0, "in consumer \n");
	struct MultiArgs *ma = (struct MultiArgs*)arg;
	//printf(0, "\nproducer is %d consumer is %d \n", *ma->sem_prod, *ma->sem_cons);
	while (ma->sum < 100) {
		sem_wait(ma->sem_cons);
		printf(0, "Consumed: %d \n", ma->sum);
		sem_post(ma->sem_prod);
	}
	exit();
}
void test_producer_consumer_init() {
  int *sem_prod = (int*)(malloc(sizeof(int)));
  int *sem_cons = (int*)(malloc(sizeof(int)));
  int *producer_stack; // the user stack for producer
  int *consumer_stack; // user stack for consumer 
  
  struct MultiArgs multi_arg;
  struct MultiArgs *ma = &multi_arg;
  ma->sum = 1; // the data to be manipulated in this test
  // sem(int* sem_id_will_be_here, 0=process 1=thread, max_number_of_subscribers)
  if(sem_init(sem_cons,0,1) < 0) {
	  printf(0,"test semaphore cons initialize: FAIL\n");
  }
  else {
	  printf(0,"test semaphore cons initialize: PASS\n");
  }

  if(sem_init(sem_prod,0,1) < 0) {
	  printf(0,"test semaphore prod initialize: FAIL\n");
  }
  else {
	  printf(0,"test semaphore prod initialize: PASS\n");
  }
  ma->sem_cons = sem_cons;
  ma->sem_prod = sem_prod;

  
  // initialize the argument going into the function executing in a thread
   //sem_post(ma->sem_cons);
  
  printf(0, "\nproducer is %d consumer is %d \n", *sem_prod, *sem_cons);
  //sleep(230); // delete, just using to see printf 
  consumer_stack = malloc(4096);
  kthread_create(&consumer, ma, consumer_stack);

  producer_stack = malloc(4096);
  kthread_create(&producer, ma, producer_stack);
 
  if(kthread_join(producer_stack) < 0)
  {printf(0,"join:FAIL\n");}
  
  if(kthread_join(producer_stack) < 0)
  {printf(0,"join:FAIL\n");}

  free(producer_stack);
  free(consumer_stack);
 
  exit();

}



int main(int argc,char **argv)
{
	printf(0, "STARTING PRODUCER-CONSUMER TEST WITH SEMAPHORE\n");	
        test_producer_consumer_init();	
    exit();
}




