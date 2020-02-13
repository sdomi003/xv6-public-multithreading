#include "types.h"
#include "user.h"

void increment(void *num)
{
   *((int*)num) = *((int*)num) + 1;
   printf(0,"increment\n");
   return;
}

int main(int argc,char **argv)
{
  int num = 1, t_pid;
  int *stack = malloc(4096);

  printf(0,"num = %d\n",num); 
  t_pid = clone(&increment,&num,&stack);
  kthread_join(&t_pid);
  printf(0,"now it equals num = %d\n",num);

  free(stack);
  return 0;
}
