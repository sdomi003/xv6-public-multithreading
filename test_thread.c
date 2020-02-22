#include "types.h"
#include "user.h"

struct multiArgs
{
   int f;
   int i;
   char *c;
};

int *sem;

void test1(void *num)
{
   int *catch = (int*)num;
   *catch = *catch + 1;

   exit();
}

void test2(void *multiarg)
{
   struct multiArgs *ma = (struct multiArgs*)multiarg;

   ma->i += 1;
   ma->c = "HJ";
   ma->f += 5;


   exit();
}

void test3(void *arg)
{
   int *num = (int*)arg;
   printf(0,"test3 thread\n");
   sem_wait(sem);
   *num += 2;
   sem_post(sem); 
   exit();
}


int main(int argc,char **argv)
{
  int num = 0;
 
  int *stack = malloc(4096);
 
  clone(&test1,&num,stack);
  printf(0,"pid = %d\n",kthread_join(stack));
  
  if (num == 1)  printf(0,"test single increment: PASS\n");
  else 		 printf(0,"test single increment: FAIL\n");
  
  struct multiArgs *ma = malloc(sizeof(struct multiArgs));

   ma->i = 2;
   ma->c = "ag";
   ma->f = 2;

   clone(&test2,ma,stack);
   printf(0,"pid = %d\n",kthread_join(stack));
  
   if (ma->i == 3 && strcmp(ma->c,"HJ") == 0 && ma->f == 7)  printf(0,"test struct multi arguments: PASS\n");
   else 
   {		 			 
	printf(0,"test struct multi argmunets: FAIL\n");
   }
	printf(0,"i = %d\n", ma->i);
        printf(0,"c = %s\n", ma->c);
	printf(0,"f = %d\n", ma->f);

  if(sem_init(sem,0,1) < 0) {printf(0,"test semaphore initialize: FAIL\n");}
  else {printf(0,"test semaphore initialize: PASS\n");} 

  int i;
  int *stack3[4]; 
  
  num = 2;
   
  for (i=0;i<4;i++)
  {
     printf(0,"alloc thread %d\n",i);
     stack3[i] = malloc(4096);
     clone(&test3,&num,stack3[i]);
  }
  printf(0," threads allocated\n");
  for (i=0;i<4;i++)
  {
     printf(0,"join thread %d\n",i);
     if(kthread_join(stack3[i]) < 0)
     	{printf(0,"test semaphore test join:FAIL\n");}
  }

  if (num == 10) {printf(0,"test semaphore increment: PASS\n");}
  else {printf(0,"test semaphore increment: FAIL\n");}

  
  for (i=0;i<4;i++)
  {
     free(stack3[i]);
  }
  
  free(ma); 
  free(stack);
  exit();
}
