#include "types.h"
#include "user.h"

void increment(void *num)
{
   *(int*)num = *(int*)num + 1;
   printf(0,"increment\n");
   exit();
}

int main(int argc,char **argv)
{
  int num = 1;
  int *stack = malloc(4096);

  printf(0,"num = %d\n",num); 
  clone(&increment,&num,stack);
  printf(0," = %d\n",kthread_join(stack));
  printf(0,"now num equals = %d\n",num);

  free(stack);
  exit();
}
