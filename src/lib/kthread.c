#include "thread.h"
#include "debug.h"
#include "x86.h"

PCB pcb[NR_PCB];
PCB* current=&pcb[0];
ListHead freelist;

//the very first pcb is for thread idle, thus no initialization is needed
ListHead *freeq_head=&freelist, *runq_head=&pcb[0].runq;

void init_pcbpool(void){
	int i=1;

	list_init(freeq_head);
        //now we orgnize the pcb array as a double list
	for(;i<NR_PCB;i++){
		list_add_after(freeq_head,&pcb[i].freeq);
	}
	assert(!list_empty(freeq_head));

	list_init(runq_head);
	assert(list_empty(runq_head));
}


PCB* create_kthread(void *entry){
	PCB* newpcb;
	TrapFrame *tf;

	//allocate a memory for the new pcb form pcb pool
	newpcb=list_entry(freeq_head->next,PCB,freeq);

//	printk("address af the allocating pcb:    %x\n",(unsigned int)newpcb);

	//one pcb is taken from the pcb pool
	list_del(freeq_head->next);

	//we need to konw the exact location in the pcb of the thread's trapframe
	tf=(TrapFrame *)(newpcb->kstack+STK_SZ)-1;
	newpcb->tf=tf;
	
	//initialize new thread's trapframe
	tf=(TrapFrame *)(newpcb->kstack+STK_SZ)-1;
	tf->ds=KSEL(SEG_KDATA);
	tf->es=KSEL(SEG_KDATA);
	tf->cs=KSEL(SEG_KCODE);
	tf->eflags=0x200;
	tf->eip=(unsigned)entry;
	
//	printk("address of the allocating pcb->tf:%x\n\n",(unsigned int)newpcb->tf);

	return newpcb;
}


void schedule(void){
	current=list_entry(current.runq->next,PCB,runq);
	if(current == &pcb[0])
		current=list_entry(current.runq->next,PCB,runq);
}

void sleep(void){
	lock();
	NOINTR;
	list_del(&current->runq);
	asm volatile("int $0x80");
	unlock();
}

void wakeup(PCB *pcb){
	lock();
	NOINTR;
	list_add_before(runq_head,&pcb->runq);
	unlock();
}

PCB* find_pcb_pid(pid_T pid){
	int i;
	for(i=0;i<NR_PCB_CREATED;i++){
		if(pcb[i].pid == pid)
		      return &pcb[i];
	}
	return NULL;
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
//		printk("sleep one\n");
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
//		printk("wake one\n");
		wakeup(pcb);
	}
	unlock();
	INTR;
}

