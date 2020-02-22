#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int
sys_clone(void)
{ 
  int function, arg, stack;
  
  if(argint(0, &function) < 0)
    return -1;
  if(argint(1, &arg) < 0)
    return -1;
  if(argint(2, &stack) < 0)
    return -1;
  return clone((void*)function,(void**)arg,(void*)stack);
}


int				
sys_kthread_join(void)
{
	int stack;
    if(argint(0, &stack) < 0)
        return -1;
    return kthread_join((void*)stack);
}

int
sys_sem_init(void)
{
	int sem,pshared,count;
    if(argint(0, &sem) < 0)
        return -1;
    if(argint(1, &pshared) < 0)
        return -1;
    if(argint(2, &count) < 0)
        return -1;
    return sem_init((int*)sem,pshared,(uint)count);
}

int
sys_sem_destroy(void)
{
	int sem;
    if(argint(0, &sem) < 0)
        return -1;
    return sem_destroy((int*)sem);
}

int
sys_sem_wait(void)
{
	int sem;
    if(argint(0, &sem) < 0)
        return -1;
    return sem_wait((int*)sem);
}

int
sys_sem_post(void)
{
	int sem;
    if(argint(0, &sem) < 0)
        return -1;
    return sem_post((int*)sem);
}
