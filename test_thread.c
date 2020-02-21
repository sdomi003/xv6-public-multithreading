#include "types.h"
#include "user.h"

struct multiArgs
{
   int f;
   int i;
   char *c;
};


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


int main(int argc,char **argv)
{
  int num = 0;
 
  int *stack = malloc(4096);
 
  clone(&test1,&num,stack);
  printf(0,"pid = %d\n",kthread_join(stack));
  
  if (num == 1)  printf(0,"test 1: PASS\n");
  else 		 printf(0,"test 1: FAIL\n");
  
  struct multiArgs *ma = malloc(sizeof(struct multiArgs));

   ma->i = 2;
   ma->c = "ag";
   ma->f = 2;

   clone(&test2,ma,stack);
   printf(0,"pid = %d\n",kthread_join(stack));
  
   if (ma->i == 3 && strcmp(ma->c,"HJ") == 0 && ma->f == 7)  printf(0,"test 2: PASS\n");
   else 
   {		 			 
	printf(0,"test 2: FAIL\n");
   }
	printf(0,"i = %d\n", ma->i);
        printf(0,"c = %s\n", ma->c);
	printf(0,"f = %d\n", ma->f);

  free(ma);
  free(stack);
  exit();
}
