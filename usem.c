// created to build the user semaphores 

// includes copied from ulib.c incase needed
// will delete unneccesary inludes when finished

#include "types.h"  // need data types
#include "stat.h" 	// probably don't need this
#include "fcntl.h"	// probably don't need this
#include "user.h"	// function definitions
#include "x86.h"	// probably don't need this


int sem_init(int *sem,int pshared, uint count)
{
	struct sem_t *s;
	int index;
	acquire(&stable.lock);
	for(index = 0; index < NPROC;  index++)
	{
		s = &stable.sem[index];

		if (s->state == SUSED){continue;}
		
		s->state = SUSED;
		s->pshared = pshared;
		s->maxcount = s->curcount = count;
		s->sid = nextsid++;
		*sem = index;
		release(&stable.lock);
		return 0;
	}

	release(&stable.lock);
	return -1;
}

int sem_destroy(int *sem)
{	
	struct sem_t *s;
	acquire(&stable.lock);
	s = &stable.sem[(*sem)];
	
	if (s->state == SUNUSED)
	{
		release(&stable.lock);
		return -1;
	}
	
	s->state = SUNUSED;
	s->pshared = 0;
	s->maxcount = s->curcount = 0;
	s->sid = 0;
	release(&stable.lock);
	return 0;
}

int sem_post(int *sem)
{
	struct sem_t *s;
	acquire(&stable.lock);
	s = &stable.sem[(*sem)];
	release(&stable.lock);
	
	if (s->state == SUNUSED) {return -1;}
	
	if (s->curcount < s->maxcount)
	{ 
		acquire(&stable.lock);
		s->curcount++; 
		release(&stable.lock);
	}
	else { return -1;}
	return 0;
}

int sem_wait(int *sem)
{
	struct sem_t *s;
	
	acquire(&stable.lock);
	s = &stable.sem[(*sem)];
	release(&stable.lock);
	
	if (s->state == SUNUSED) {return -1;}
	while(1)
	{ 
		acquire(&stable.lock);
		if (s->curcount <= 0) 
		{
			release(&stable.lock);
			yield(); 
		}
		else 
		{
			s->curcount--;
			release(&stable.lock);
			break;
		}
	}
	
	return 0;
}
