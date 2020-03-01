#include "types.h"
#include "user.h"

struct MultiArgs
{
   int sum;
   int *sem;
};



void sem_test_1(void *arg)
{
   struct MultiArgs *ma = (struct MultiArgs*)arg;
   printf(0,"sem_test_1 thread\n");
   int *sem = ma->sem;
   sem_wait(sem);
   ma->sum += 2;
   sem_post(sem); 
   exit();
}

void sem_test_1_init(){
  int *sem = 0; // pass into init and sem will be updated with the sem_id
  int i;
  int *stacks[4]; // the user stacks for the 4 threads  
  
  struct MultiArgs multi_arg;
  struct MultiArgs *ma = &multi_arg;
  // sem(int* sem_id_will_be_here, 0=process 1=thread, max_number_of_subscribers)
  if(sem_init(sem,0,1) < 0) {
	  printf(0,"test semaphore 1 initialize: FAIL\n");
  }
  else {
	  printf(0,"test semaphore 1 initialize: PASS\n");
  } 

  // initialize the argument going into the function executing in a thread
  ma->sem = sem;
  ma->sum = 2; // the data to be manipulated in this test
     
  for (i=0;i<4;i++)
  {
     printf(0,"alloc thread %d\n",i);
     stacks[i] = malloc(4096);
     clone(&sem_test_1,ma,stacks[i]);
  }
  printf(0,"4 threads allocated\n");
  for (i=0;i<4;i++)
  {
     printf(0,"join thread %d\n",i);
     if(kthread_join(stacks[i]) < 0)
     	{printf(0,"test semaphore 1 join:FAIL\n");}
  }

  if (ma->sum == 10) {printf(0,"test semaphore 1 increment: PASS\n");}
  else {printf(0,"test semaphore increment: FAIL\n");}

  
  for (i=0;i<4;i++)
  {
     free(stacks[i]);
  }
  
  exit();

}

int main(int argc,char **argv)
{
	sem_test_1_init(); 
	exit(); 
}




