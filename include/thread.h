#ifndef __KTHREAD_H__
#define __KTHREAD_H__

#include "assert.h"
#include "common.h"
#include "adt/list.h"
#include "x86.h"
#include "message.h"
#include "semaphore.h"

//size of stack in each PCB
#define STK_SZ     	0x1000
#define MSG_SZ		1024
#define NR_PCB		128	
#define ANY		-1
#define MSG_HWINTR	-2
/*
//status of kernel thread
#define KTH_FREE 	0
#define KTH_NRUNNABLE 	1
#define KTH_RUNNABLE 	2
*/



//pcb for thread
struct PCB{
	//trap frame pointer pointing to register contest
	TrapFrame	*tf;		
	int32_t  	pid;		//kernel thread id
	//uint32_t 	kthsta;		//kernel thread status

	//lock_count for each pcb
	int32_t  	lock_count;
	Semaphore	message;
	Message*	message_addr;
	
	//runq: manages runable pcb using list_head
	ListHead 	runq, freeq;

	//manages threads whick sleep
	ListHead 	semq;	

	//stack for each pcb
	char 		kstack[STK_SZ];	//stack in PCB
};
typedef struct PCB PCB;

extern PCB* current;

void init_pcbpool(void);
void schedule(void);
PCB* find_pcb_pid(pid_t pid);
PCB* create_kthread(void *entry);
void sleep(void);
void wakeup(PCB *pcb);
PCB* set_pid(PCB *pcb,pid_t pid);


static inline void
lock(void){
	cli();                  //notice we must clear IF first
        current->lock_count++;
}

static inline void
unlock(void){
        current->lock_count--;
        assert(current->lock_count>=0);
        if(current->lock_count==0)
	      sti();           //notice we must set IF at the very last
}


#define INTR assert(readf() & FL_IF)

#define NOINTR assert(~readf() & FL_IF)

#endif
