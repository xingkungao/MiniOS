#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include "thread.h"

struct Semaphore{
	int count;
	ListHead queue;
};
typedef struct Semaphore Semaphore;

void new_sem(Semaphore* sem,int count);

void P(Semaphore* sem);

void V(Semaphore* sem);

#endif
