#include "thread.h"
#include "debug.h"
#include "x86.h"

PCB pcb[NR_PCB];
PCB* current=&pcb[0];
ListHead freelist;
ListHead *freeq_head=&freelist, *runq_head=&pcb[0].runq;

void init_pcbpool(void){
	int i=1;

	list_init(freeq_head);

	for(;i<NR_PCB;i++){
		list_add_after(freeq_head,&pcb[i].freeq);
	}
	assert(!list_empty(freeq_head));

	list_init(runq_head);
	assert(list_empty(runq_head));
}


PCB* create_kthread(void *entry){
//	int i=1;
	PCB* newpcb;
	TrapFrame *tf;
/*	if(NULL==freeq_head->next){
		list_init(freeq_head);
//		assert(list_empty(freeq_head)!=1);
//		for(;i<NR_PCB;i++){
//			pcb[i].freeq.next=&pcb[(i+1+NR_PCB)%NR_PCB].freeq;
//			pcb[i].freeq.prev=&pcb[(i-1+NR_PCB)%NR_PCB].freeq;
//		}
		for(;i<NR_PCB;i++){
			list_add_after(freeq_head,&pcb[i].freeq);
		}
		assert(list_empty(freeq_head)!=1);
	}

	if(NULL==runq_head->next || NULL==runq_head->prev)
	      list_init(runq_head);
	

*/
	//allocate a memory for the new pcb form pcb pool
	newpcb=list_entry(freeq_head->next,PCB,freeq);

	printk("address af the allocating pcb:    %x\n",(unsigned int)newpcb);
	list_del(freeq_head->next);

	//initialize new thread;
	tf=(TrapFrame *)(newpcb->kstack+STK_SZ)-1;
	newpcb->tf=tf;
	tf->ds=KSEL(SEG_KDATA);
	tf->es=KSEL(SEG_KDATA);
	tf->cs=KSEL(SEG_KCODE);
	tf->eflags=0x200;
	tf->eip=(unsigned)entry;
	
	printk("address of the allocating pcb->tf:%x\n\n",(unsigned int)newpcb->tf);

	//list_add_before(runq_head,&newpcb->runq);

	//assert(!list_empty(runq_head));

	return newpcb;
}


void sleep(void){
	lock();
	NOINTR;
	list_del(&current->runq);
	asm volatile("int $0x80");
	unlock();
	INTR;
}

void wakeup(PCB *pcb){
//	lock();
	list_add_before(runq_head,&pcb->runq);
//	lock();
}

void 
new_sem(Semaphore *sem,int value){
	sem->count=value;
	list_init(&sem->queue);
}

void 
P(Semaphore *sem){
	lock();
	NOINTR;
	sem->count--;
	if(sem->count < 0){
		list_add_before(&sem->queue,&current->semq);
		printk("sleep one\n");
		sleep();
	}
	unlock();
	INTR;
}

void 
V(Semaphore *sem){
	lock();
	NOINTR;
	sem->count++;
	if(sem->count <= 0){
		assert(!list_empty(&sem->queue));
		PCB *pcb=list_entry(sem->queue.next,PCB,semq);
		list_del(sem->queue.next);
		printk("wake one\n");
		wakeup(pcb);
	}
	unlock();
	INTR;
}

