#ifndef __KTHREAD_H__
#define __KTHREAD_H__

#include "assert.h"
#include "common.h"
#include "adt/list.h"
#include "x86.h"

//size of stack in each PCB
#define STK_SZ     	0x1000
#define MSG_SZ		1024

//number of PCB
#define NR_PCB		128	
/*
//status of kernel thread
#define KTH_FREE 	0
#define KTH_NRUNNABLE 	1
#define KTH_RUNNABLE 	2
*/

struct Message{
	pid_t src,dest;
	size_t type;
	ListHead messq;
	char payload[1024];
}
typedef struct Message Message;

//semaphore to manage sleeped threads.
struct Semaphore{
	int count;
	ListHead queue;
};
typedef struct Semaphore Semaphore;


//pcb for thread
struct PCB{
	//trap frame pointer pointing to register contest
	TrapFrame	*tf;		
	int32_t  	pid;		//kernel thread id
	//uint32_t 	kthsta;		//kernel thread status

	//lock_count for each pcb
	int32_t  	lock_count;
	Semaphore	message;
	uint32_t	message_addr;
	
	//runq: manages runable pcb using list_head
	ListHead 	runq, freeq;

	//manages threads whick sleep
	ListHead 	semq;	

	//stack for each pcb
	char 		kstack[STK_SZ];	//stack in PCB
};
typedef struct PCB PCB;

extern PCB* current;

//manage a memory pool to create new pcb
void init_pcbpool(void);

//create a kernel thread
PCB* create_kthread(void *entry);

//sleep a kernel thread
void sleep(void);

//wake up a kernel thread
void wakeup(PCB *pcb);

static inline void
lock(void){
	cli();			//notice we must clear IF first
	current->lock_count++;
}
static inline void
unlock(void){
	current->lock_count--;
	assert(current->lock_count>=0);
	if(current->lock_count==0)
	      sti();		//notice we must set IF at the very last
}


#define	INTR assert(readf() & FL_IF)

#define	NOINTR assert(~readf() & FL_IF)


void new_sem(Semaphore* sem,int count);

void P(Semaphore* sem);

void V(Semaphore* sem);

void send(int pid, struct message *m);
void receive(int pid, struct message *m);


#endif
