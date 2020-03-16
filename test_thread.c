#include "types.h"
#include "user.h"


struct multiArgs
{
   int f;
   int i;
   char *c;
};

int *r, *w, *z;

// single thread catch and increment int argument
void test1(void *num)
{
   int *catch = (int*)num;
   *catch = *catch + 1;

   exit();
}

// passing a struct with multiple arguments and assigning new values
void test2(void *multiarg)
{
   struct multiArgs *ma = (struct multiArgs*)multiarg;

   ma->i += 1;
   ma->c = "HJ";
   ma->f += 5;


   exit();
}

// writer for reader/writer test
void test3_w(void *arg)
{
   int *num = (int*)arg;
   sem_wait(r);
   sem_wait(w);
   *num += 2;
   sem_post(w);
   sem_post(r); 
   exit();
}

// reader for reader/writer test
void test3_r(void *arg)
{
   int *num = (int*)arg;
   sem_wait(r);
   printf(0,"reader thread num = %d\n",*num);    
   sem_post(r);
   exit();
}

void inner_recursive_thread(void *arg) {
	int *num_threads = (int*) arg;
	printf(0, "\n INNER THREAD # %d checking in. \n", (int) *num_threads);
	*num_threads = *num_threads - 1;	
	if(*num_threads > 0){
		int *stack_inner = malloc(4096);
		kthread_create(&inner_recursive_thread, num_threads, stack_inner);
		kthread_join(stack_inner);
		free(stack_inner);
	}
	exit();
}
void init_recursive_test(int *num_threads) {	
	printf(0, "\nGoing to recursively create %d threads.\n", *num_threads);
	int *stack_inner = malloc(4096);
	kthread_create (&inner_recursive_thread, num_threads, stack_inner);	
	kthread_join(stack_inner);
	free(stack_inner);
	//sleep(1000);
	exit();
}

int main(int argc,char **argv)
{
  int num = 0;
 
  int *stack = malloc(4096);
 
  kthread_create(&test1,&num,stack);
  printf(0,"pid = %d\n",kthread_join(stack));
  
  if (num == 1)  printf(0,"test single increment: PASS\n");
  else 		 printf(0,"test single increment: FAIL\n");
  
  struct multiArgs *ma = malloc(sizeof(struct multiArgs));

   ma->i = 2;
   ma->c = "ag";
   ma->f = 2;

   kthread_create(&test2,ma,stack);
   printf(0,"pid = %d\n",kthread_join(stack));
  
   if (ma->i == 3 && strcmp(ma->c,"HJ") == 0 && ma->f == 7)  printf(0,"test struct multi arguments: PASS\n");
   else 
   {		 			 
	printf(0,"test struct multi argmunets: FAIL\n");
   }
	printf(0,"i = %d\n", ma->i);
        printf(0,"c = %s\n", ma->c);
	printf(0,"f = %d\n", ma->f);
  w = malloc(sizeof(int));
  r = malloc(sizeof(int));  

  //*w = sem_init(w,0,1);
  if(sem_init(w,0,1) < 0) {printf(0,"test semaphore initialize: FAIL\n");}
  else {printf(0,"test semaphore initialize: PASS\n");} 

  //*r = sem_init(r,0,1);
  if(sem_init(r,0,1) < 0) {printf(0,"test semaphore initialize: FAIL\n");}
  else {printf(0,"test semaphore initialize: PASS\n");} 

  printf(0,"r = %d\n",*r);
  printf(0,"w = %d\n",*w);


  int i;
  int *stack3[4]; 
  
  num = 2;
   
  for (i=0;i<3;i++)
  {
     printf(0,"alloc thread %d\n",i);
     stack3[i] = malloc(4096);
     kthread_create(&test3_w,&num,stack3[i]);
  }
  stack3[3] = malloc(4096);
  kthread_create(&test3_r,&num,stack3[3]);

  printf(0," threads allocated\n");
  for (i=0;i<4;i++)
  {
     printf(0,"join thread %d\n",i);
     if(kthread_join(stack3[i]) < 0)
     	{printf(0,"test semaphore join:FAIL\n");}
  }

  if (num == 8) {printf(0,"test semaphore increment: PASS\n");}
  else {printf(0,"test semaphore increment: FAIL\n");}
 
 
  for (i=0;i<4;i++)
  {
     free(stack3[i]);
  }
  sem_destroy(r);
  sem_destroy(w);
 
  free(r);
  free(w);
  free(ma); 
  free(stack);
  
  printf(0,"");

  // test recursive threads
  int *num_threads = (int*)(malloc(sizeof(int)));
  *num_threads = 10;
  init_recursive_test(num_threads);
  
  exit();
}
