#ifndef __KTHREAD_H__
#define __KTHREAD_H__

#include "assert.h"
#include "common.h"
#include "adt/list.h"
#include "x86.h"

//size of stack in each PCB
#define STK_SZ     	0x1000

//number of PCB
#define NR_PCB		16

//status of kernel thread
#define KTH_FREE 	0
#define KTH_NRUNNABLE 	1
#define KTH_RUNNABLE 	2

//pcb for thread
struct PCB{
	TrapFrame *tf;		//trape frame pointer to save registers
	int32_t kthid;		//kernel thread id
	uint32_t kthsta;	//kernel thread status
//	uint32_t cr3;
	ListHead runq, freeq;	//runable queue and free queue
	ListHead semq;		//semaphor queue
	char kstack[STK_SZ];	//stack in PCB
};
typedef struct PCB PCB;

//semaphore for schedule
struct Semaphore{
	int count;
	ListHead queue;
};
typedef struct Semaphore Semaphore;

//allocate a pool for pcb available list
void init_pcbpool(void);

//create a kernel thread
PCB* create_kthread(void *entry);

//sleep a kernel thread
void sleep(void);

//wake up a kernel thread
void wakeup(PCB *pcb);

//void lock(void);
//void unlock(void);
#define lock()   do{ cli(); } while(0)
#define	INTR assert(~readf() & FL_IF)
#define unlock() do{ sti(); } while(0)
#define	NOINTR assert(readf() & FL_IF)


void new_sem(Semaphore* sem,int count);

void P(Semaphore* sem);

void V(Semaphore* sem);



#endif
